package piston.debugger.http;

import java.net.InetSocketAddress;

import piston.debugger.PistonDebugTarget;

import com.sun.net.httpserver.HttpServer;


public class PistonHTTPServerThread extends Thread
{
    protected PistonDebugTarget debugTarget;
    protected String host;
    protected int port;
    
    public PistonHTTPServerThread(PistonDebugTarget debugTarget, String host, int port)
    {
        super();
        
        this.debugTarget = debugTarget;
        this.host = host;
        this.port = port;
    }
    
    public void run()
    {
        try
        {
            HttpServer server = HttpServer.create(new InetSocketAddress(host, port), 0);
            server.createContext("/", new PistonHTTPServerHandler(debugTarget));
            server.setExecutor(null);
            server.start();
            
            while(!debugTarget.isTerminated())
            {
                Thread.sleep(500);
            }
            
            server.stop(0);
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
}
