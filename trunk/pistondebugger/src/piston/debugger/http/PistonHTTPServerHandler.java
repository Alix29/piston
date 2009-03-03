package piston.debugger.http;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;

import piston.debugger.PistonDebugTarget;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;


public class PistonHTTPServerHandler implements HttpHandler
{
    protected PistonDebugTarget debugTarget;
    
    public PistonHTTPServerHandler(PistonDebugTarget debugTarget)
    {
        this.debugTarget = debugTarget;
    }
    
    public void handle(HttpExchange exchange) throws IOException
    {
        String requestURI = exchange.getRequestURI().toString();
        BufferedReader br = new BufferedReader(new InputStreamReader(exchange.getRequestBody()));
        
        StringBuffer reply = new StringBuffer(); 
        if(!debugTarget.receivedDebuggerMessage(requestURI.substring(1), br, reply))
            reply.append("ERROR");
        
        exchange.sendResponseHeaders(200, reply.length());
        
        OutputStream os = exchange.getResponseBody();
        os.write(reply.toString().getBytes());
        os.close();
    }
}
