package piston.debugger;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.Socket;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.eclipse.core.resources.IMarker;
import org.eclipse.core.resources.IMarkerDelta;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Status;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.IBreakpointManager;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IBreakpoint;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IMemoryBlock;
import org.eclipse.debug.core.model.IProcess;
import org.eclipse.debug.core.model.IThread;

import piston.debugger.http.PistonHTTPServerThread;

public class PistonDebugTarget extends DebugElement implements IDebugTarget
{
    protected ILaunch                   launch;
    protected Process                   p;
    protected IProcess                  rp;
    protected PistonHTTPServerThread    httpServerThread;
    protected String                    remoteAddress;
    protected int                       remotePort;
    protected List<PistonDebugThread>     threads = new ArrayList<PistonDebugThread>();
    protected ILaunchConfiguration      configuration;
    protected boolean                   terminated=false;

    public PistonDebugTarget(ILaunch launch, ILaunchConfiguration configuration) throws CoreException
    {
        super(null);
        
        this.launch = launch;
        this.configuration = configuration;
        
        fireCreationEvent();
    }
    
    public void start() throws CoreException
    {
        // TODO: Find a suitable address to listen on instead of just picking
        remoteAddress = "localhost";
        remotePort = 7570;
        
        // Launch our process
        String appPath = configuration.getAttribute("appPath", "");
        if(appPath.length()<1)
            throw new CoreException( new Status(IStatus.ERROR, PistonActivator.PLUGIN_ID, IStatus.OK, "No app path set, failing.", null));

        String jsFilePath = configuration.getAttribute("jsFilePath", "");
        if(jsFilePath.length()<1)
            throw new CoreException( new Status(IStatus.ERROR, PistonActivator.PLUGIN_ID, IStatus.OK, "No javascript path set, failing.", null));

        List<String> launchCommand = new ArrayList<String>();
        launchCommand.add(appPath);
        launchCommand.add("--enableDebugger");
        launchCommand.add(jsFilePath);
            
        ProcessBuilder pb = new ProcessBuilder(launchCommand);
        
        String tempWdPath = appPath.substring(0, appPath.lastIndexOf(File.separatorChar));
        tempWdPath = tempWdPath.substring(0, tempWdPath.lastIndexOf(File.separatorChar));
        
        pb.directory(new File(tempWdPath));
        
        Map<String, String> processAttributes = new HashMap<String, String>();
        
        try
        {
            p = pb.start();
            rp = DebugPlugin.newProcess(launch, p, appPath.indexOf(File.separatorChar)!=-1 ? appPath.substring(appPath.lastIndexOf(File.separatorChar)) : appPath, processAttributes);
        }
        catch(Exception e)
        {
            throw new CoreException( new Status(IStatus.ERROR, PistonActivator.PLUGIN_ID, IStatus.OK, "Failure launching process", e));
        }
        
        // loop until we connect to our launched process
        boolean successfulConnect = false;
        int tries = 0;
        while(!successfulConnect && tries<100)
        {
            try
            {
                Socket sock = new Socket(remoteAddress, remotePort);
                successfulConnect = true;
                sock.close();
            }
            catch(Exception e)
            {
                tries++;
                try
                {
                    Thread.sleep(200);
                }
                catch(Exception ee)
                {
                    
                }
            }
        }
        
        if(!successfulConnect)
            throw new CoreException(new Status(IStatus.ERROR, PistonActivator.PLUGIN_ID, IStatus.OK, "Failure to connect to process debugger", new Throwable()));        
        
        httpServerThread = new PistonHTTPServerThread(this, p);
        httpServerThread.start();
        
        // Send any existing breakpoints we have
        IBreakpointManager breakpointManager = DebugPlugin.getDefault().getBreakpointManager();
        IBreakpoint[] breakpoints = breakpointManager.getBreakpoints();
        for(IBreakpoint breakpoint : breakpoints)
        {
            breakpointAdded(breakpoint);
        }

        DebugPlugin.getDefault().getBreakpointManager().addBreakpointListener(this);

        startDebugger();
    }
    
    public Boolean receivedDebuggerMessage(String messageName, BufferedReader messageData, StringBuffer reply) throws IOException
    {
        if(messageName.equals("breakpoint_reached"))
        {
            int threadNum = Integer.parseInt(messageData.readLine());
            String filename = messageData.readLine();
            int line = Integer.parseInt(messageData.readLine());
            
            PistonDebugThread thread = getThreadByNum(threadNum);
            if(thread!=null)
                thread.handleReachedBreakpoint(new Path(filename).toFile(), line);
            else
                return false;

            return true;
        }
        else if(messageName.equals("stepped"))
        {
            int threadNum = Integer.parseInt(messageData.readLine());
            String filename = messageData.readLine();
            int line = Integer.parseInt(messageData.readLine());
            
            PistonDebugThread thread = getThreadByNum(threadNum);
            if(thread!=null)
                thread.handleStepped(new Path(filename).toFile(), line);
            else
                return false;

            return true;
        }
        else if(messageName.equals("thread_created"))
        {
            String threadName = messageData.readLine();
            int threadNum = Integer.parseInt(messageData.readLine());
            
            PistonDebugThread thread = new PistonDebugThread(this, threadName, threadNum);
            threads.add(thread);
            
            thread.fireCreationEvent();
        }
        
        return false;
    }
    
    public PistonDebugThread getThreadByNum(int threadNum)
    {
        for(PistonDebugThread thread : threads)
        {
            if(thread.threadNum==threadNum)
                return thread;
        }
        
        return null;
    }
    
    public List<String> sendDebuggerMessage(String messageName)
    {
        return sendDebuggerMessage(messageName, "\n");
    }
    
    public List<String> sendDebuggerMessage(String messageName, String messageData)
    {
        List<String> result = new ArrayList<String>();
        
        try
        {
            byte[] postDataBytes = messageData.getBytes();
            HttpURLConnection con = (HttpURLConnection)new URL("http://" + remoteAddress + ":" + remotePort + "/" + messageName).openConnection();
            con.setRequestMethod("POST");
            con.setRequestProperty("Content-Length", Integer.toString(postDataBytes.length));
            con.setInstanceFollowRedirects(false);
            con.setDoInput(true);
            con.setDoOutput(true);
            con.setUseCaches(false);
            con.connect();
    
            OutputStream os = con.getOutputStream();
            os.write(postDataBytes);
            os.close();
            
            InputStream is = con.getInputStream();
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            for(String line=br.readLine();line!=null;result.add(line),line=br.readLine())
            ;
            is.close();

        }
        catch(Exception e)
        {
    
        }
        
        return result;
    }

    @Override
    public String getName() throws DebugException
    {
        return "Piston Local Application";
    }

    @Override
    public IProcess getProcess()
    {
        return rp;
    }

    @Override
    public IThread[] getThreads() throws DebugException
    {
        IThread[] result = threads.toArray(new IThread[0]);
        return result;
    }

    @Override
    public boolean hasThreads() throws DebugException
    {
        return threads.size()>0;
    }

    @Override
    public boolean supportsBreakpoint(IBreakpoint breakpoint)
    {
        return true;
    }

    @Override
    public String getModelIdentifier()
    {
        return PistonActivator.PLUGIN_ID;
    }

    @Override
    public boolean canTerminate()
    {
        return !terminated;
    }

    @Override
    public boolean isTerminated()
    {
        return terminated;
    }

    @Override
    public void terminate() throws DebugException
    {
        // TODO: Handle Better?
        terminated = true;

        for(PistonDebugThread thread : threads)
        {
             thread.terminate();
        }
        
        if(p!=null)
            p.destroy();

        if(rp!=null)
            rp.terminate();
        
        fireTerminateEvent();
        launch.terminate();
        
        DebugPlugin.getDefault().getBreakpointManager().removeBreakpointListener(this);        
    }

    @Override
    public boolean canResume()
    {
        return false;
    }

    @Override
    public boolean canSuspend()
    {
        return false;
    }

    @Override
    public boolean isSuspended()
    {
        return false;
    }

    @Override
    public void resume() throws DebugException
    {
        
    }

    @Override
    public void suspend() throws DebugException
    {
        
    }

    @Override
    public void breakpointAdded(IBreakpoint breakpoint)
    {
        IMarker marker = breakpoint.getMarker();

        IResource resource = marker.getResource();
        IPath path = resource.getRawLocation();
        String filepath = path.makeAbsolute().toOSString();

        try
        {
            sendDebuggerMessage("breakpoint_set", filepath + "\n" + marker.getAttribute(IMarker.LINE_NUMBER).toString() + "\n");
        }
        catch(Exception e)
        {

        }
    }

    @Override
    public void breakpointChanged(IBreakpoint breakpoint, IMarkerDelta delta)
    {
        // TODO Auto-generated method stub

    }

    @Override
    public void breakpointRemoved(IBreakpoint breakpoint, IMarkerDelta delta)
    {
        // TODO Auto-generated method stub

    }

    public void startDebugger()
    {
        sendDebuggerMessage("start");
    }

    @Override
    public boolean canDisconnect()
    {
        return false;
    }

    @Override
    public void disconnect() throws DebugException
    {

    }

    @Override
    public boolean isDisconnected()
    {
        return false;
    }

    @Override
    public IMemoryBlock getMemoryBlock(long startAddress, long length) throws DebugException
    {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public boolean supportsStorageRetrieval()
    {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public IDebugTarget getDebugTarget()
    {
        return this;
    }

    @Override
    public ILaunch getLaunch()
    {
        return launch;
    }

    @Override
    public Object getAdapter(Class adapter)
    {
        return super.getAdapter(adapter);
    }
}