package piston.debugger;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.codec.binary.Base64;
import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IRegisterGroup;
import org.eclipse.debug.core.model.IStackFrame;
import org.eclipse.debug.core.model.IThread;
import org.eclipse.debug.core.model.IVariable;

public class PistonDebugStackFrame extends DebugElement implements IStackFrame
{
    protected PistonDebugTarget debugTarget;
    protected PistonDebugThread thread;
    protected String filename;
    protected int lineNum;
    protected List<PistonDebugVariable> variables = new ArrayList<PistonDebugVariable>();
    
    public PistonDebugStackFrame(PistonDebugTarget debugTarget, PistonDebugThread thread, String filename, int lineNum)
    {
        super(debugTarget);
        
        this.debugTarget = debugTarget;
        this.thread = thread;
        this.filename = filename;
        this.lineNum = lineNum;
        
        // Get our variables
        List<String> variablesResult = debugTarget.sendDebuggerMessage("variables_get", "" + thread.threadNum + "\n");
        for(String variableResult : variablesResult)
        {
            String[] variableProps = variableResult.split("\t");
            if(variableProps.length!=4)
                continue;
            
            String value = variableProps[3];
            byte[] valueBytes = Base64.decodeBase64(value.getBytes());
            String valueDecoded = ""; 
            try
            {
                valueDecoded = new String(valueBytes, "UTF-8");
            }
            catch(Exception e)
            {
                valueDecoded = new String(valueBytes);
            }
            
            //Base64.decode(value.getBytes())
            
            PistonDebugValue variableValue = new PistonDebugValue(debugTarget, thread, variableProps[1], valueDecoded, Boolean.parseBoolean(variableProps[2]));
            PistonDebugVariable variable = new PistonDebugVariable(debugTarget, thread, variableProps[0], variableValue);
            variables.add(variable);
        }
    }

    @Override
    public int getCharEnd() throws DebugException
    {
        return -1;
    }

    @Override
    public int getCharStart() throws DebugException
    {
        return -1;
    }

    @Override
    public int getLineNumber() throws DebugException
    {
        return lineNum;
    }

    @Override
    public String getName() throws DebugException
    {
        return filename + ":" + lineNum;
    }

    @Override
    public IRegisterGroup[] getRegisterGroups() throws DebugException
    {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public IThread getThread()
    {
        return thread;
    }

    @Override
    public IVariable[] getVariables() throws DebugException
    {
        return variables.toArray(new IVariable[0]);
    }

    @Override
    public boolean hasRegisterGroups() throws DebugException
    {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean hasVariables() throws DebugException
    {
        return variables.size()>0;
    }

    @Override
    public String getModelIdentifier()
    {
        return thread.getModelIdentifier();
    }

    @Override
    public boolean canStepInto()
    {
        return thread.canStepInto();
    }

    @Override
    public boolean canStepOver()
    {
        return thread.canStepOver();
    }

    @Override
    public boolean canStepReturn()
    {
        return thread.canStepReturn();
    }

    @Override
    public boolean isStepping()
    {
        return thread.isStepping();
    }

    @Override
    public void stepInto() throws DebugException
    {
        thread.stepInto();
    }

    @Override
    public void stepOver() throws DebugException
    {
        thread.stepOver();
    }

    @Override
    public void stepReturn() throws DebugException
    {
        thread.stepReturn();
    }

    @Override
    public boolean canResume()
    {
        return thread.canResume();
    }

    @Override
    public boolean canSuspend()
    {
        return thread.canSuspend();
    }

    @Override
    public boolean isSuspended()
    {
        return thread.isSuspended();
    }

    @Override
    public void resume() throws DebugException
    {
        thread.resume();
    }

    @Override
    public void suspend() throws DebugException
    {
        thread.suspend();
    }

    @Override
    public boolean canTerminate()
    {
        return thread.canTerminate();
    }

    @Override
    public boolean isTerminated()
    {
        return thread.isTerminated();
    }

    @Override
    public void terminate() throws DebugException
    {
        thread.terminate();
    }
}