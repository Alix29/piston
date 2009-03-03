package piston.debugger.http;

import java.net.InetSocketAddress;

import piston.debugger.PistonDebugTarget;

import com.sun.net.httpserver.HttpServer;


public class PistonHTTPServerThread extends Thread
{
    protected Process p;
    protected PistonDebugTarget debugTarget;
    
    public PistonHTTPServerThread(PistonDebugTarget debugTarget, Process p)
    {
        super();
        
        this.debugTarget = debugTarget;
        this.p = p;
    }
    
    public void run()
    {
        try
        {
            HttpServer server = HttpServer.create(new InetSocketAddress(7580), 0);
            server.createContext("/", new PistonHTTPServerHandler(debugTarget));
            server.setExecutor(null);
            server.start();
            
            p.waitFor();
            
            server.stop(0);
            debugTarget.terminate();
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
}
