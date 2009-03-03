package piston.debugger;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.eclipse.core.resources.IMarker;
import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.IBreakpointManager;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IBreakpoint;
import org.eclipse.debug.core.model.IDebugTarget;
import org.eclipse.debug.core.model.IStackFrame;
import org.eclipse.debug.core.model.IThread;

public class PistonDebugThread extends DebugElement implements IThread
{
    public PistonDebugTarget  debugTarget;
    public String           threadName;
    public int              threadNum;
    public boolean          suspended=false;
    public boolean          stepping=false;
    public boolean          terminated=false;
    public IBreakpoint      breakpointReached=null;
    public List<PistonDebugStackFrame>    stackFrames=new ArrayList<PistonDebugStackFrame>();
    
    public PistonDebugThread(PistonDebugTarget debugTarget, String threadName, int threadNum)
    {
        super(debugTarget);
        
        this.debugTarget = debugTarget;
        this.threadName = threadName;
        this.threadNum = threadNum;
    }
    
    public void handleStepped(File file, int line)
    {
        suspended = true;
       
        // Get our stack frames
        updateInternalStackFrames();
        
        fireSuspendEvent(DebugEvent.STEP_END);
        fireChangeEvent(DebugEvent.STATE);
    }
    
    public void handleReachedBreakpoint(File file, int line)
    {
        int smallestDifference=Integer.MAX_VALUE;
        IBreakpoint closestBreakpoint=null;
        
        suspended = true;
        
        // Get our stack frames
        updateInternalStackFrames();
        
        // Get which breakpoint we are at
        IBreakpointManager breakpointManager = DebugPlugin.getDefault().getBreakpointManager();
        IBreakpoint[] breakpoints = breakpointManager.getBreakpoints();
        for(IBreakpoint breakpoint : breakpoints)
        {
            IMarker breakpointMarker = breakpoint.getMarker();
            File breakpointFile = breakpointMarker.getResource().getLocation().toFile();
            
            try
            {
                if(!breakpointFile.getCanonicalFile().equals(file.getCanonicalFile()))
                    continue;
                
                int breakpointLine = Integer.parseInt(breakpointMarker.getAttribute(IMarker.LINE_NUMBER).toString());
                if((line-breakpointLine)<smallestDifference)
                    closestBreakpoint = breakpoint;
            }
            catch(Exception e)
            {
                
            }
        }
        
        int debugDetail = DebugEvent.UNSPECIFIED;
        
        if(closestBreakpoint!=null)
        {
            breakpointReached = closestBreakpoint;
            debugDetail = DebugEvent.BREAKPOINT;
            DebugPlugin.getDefault().getBreakpointManager().fireBreakpointChanged(closestBreakpoint);
        }
        
        fireSuspendEvent(debugDetail);
        fireChangeEvent(DebugEvent.STATE);
    }
    
    public void updateInternalStackFrames()
    {
        stackFrames.clear();
        
        List<String> stringStackFrames = debugTarget.sendDebuggerMessage("stack_get", "" + threadNum + "\n");
        //List<String> stringStackFrames = new ArrayList<String>();
        //stringStackFrames.add("abc.c:4");
        for(String stringStackFrame : stringStackFrames)
        {
            String[] stringStackFrameParts = stringStackFrame.split(":");
            if(stringStackFrameParts.length==2)
            {
                PistonDebugStackFrame stackFrame = new PistonDebugStackFrame(debugTarget, this, stringStackFrameParts[0], Integer.parseInt(stringStackFrameParts[1]));
                stackFrames.add(stackFrame);
            }
        }
    }
    
    @Override
    public IBreakpoint[] getBreakpoints()
    {
        return (breakpointReached!=null ? new IBreakpoint[] { breakpointReached } : new IBreakpoint[0]);
    }

    @Override
    public String getName() throws DebugException
    {
        return "" + threadNum + " " + threadName + (suspended ? " (Suspended)" : "");
    }

    @Override
    public int getPriority() throws DebugException
    {
        return 0;
    }

    @Override
    public IStackFrame[] getStackFrames() throws DebugException
    {
        return stackFrames.toArray(new IStackFrame[0]);
    }

    @Override
    public IStackFrame getTopStackFrame() throws DebugException
    {
        return stackFrames.size()<1 ? null : stackFrames.get(0);
    }

    @Override
    public boolean hasStackFrames() throws DebugException
    {
        return stackFrames.size()>0;
    }

    @Override
    public IDebugTarget getDebugTarget()
    {
        return debugTarget;
    }

    @Override
    public ILaunch getLaunch()
    {
        return debugTarget.getLaunch();
    }

    @Override
    public String getModelIdentifier()
    {
        return debugTarget.getModelIdentifier();
    }

    @Override
    public Object getAdapter(Class adapter)
    {
        return super.getAdapter(adapter);
    }

    @Override
    public boolean canResume()
    {
        return !terminated && suspended;
    }

    @Override
    public boolean canSuspend()
    {
        //return !terminated && !suspended;
        return false;
    }

    @Override
    public boolean isSuspended()
    {
        return !terminated && suspended;
    }

    @Override
    public void resume() throws DebugException
    {
        if(terminated || !suspended)
            return;
        
        stepping = false;
        suspended = false;
        stackFrames.clear();
        fireResumeEvent(DebugEvent.CLIENT_REQUEST);
        fireChangeEvent(DebugEvent.STATE);

        debugTarget.sendDebuggerMessage("continue", "" + threadNum + "\n");
    }

    @Override
    public void suspend() throws DebugException
    {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean canStepInto()
    {
        return true;
    }

    @Override
    public boolean canStepOver()
    {
        return true;
    }

    @Override
    public boolean canStepReturn()
    {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean isStepping()
    {
        return stepping;
    }

    @Override
    public void stepInto() throws DebugException
    {
        stepping = true;
        debugTarget.sendDebuggerMessage("step_into", "" + threadNum + "\n");
    }

    @Override
    public void stepOver() throws DebugException
    {
        stepping = true;
        debugTarget.sendDebuggerMessage("step_over", "" + threadNum + "\n");
    }

    @Override
    public void stepReturn() throws DebugException
    {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean canTerminate()
    {
        // TODO: Add checks?
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
        // TODO Implement better
        stepping = false;
        suspended = false;
        terminated = true;
        fireTerminateEvent();
    }
}