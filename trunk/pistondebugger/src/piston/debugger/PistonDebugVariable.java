package piston.debugger;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IValue;
import org.eclipse.debug.core.model.IVariable;


public class PistonDebugVariable extends DebugElement implements IVariable
{
    protected PistonDebugTarget debugTarget;
    protected PistonDebugThread thread;
    protected String name;
    protected PistonDebugValue value;
    
    public PistonDebugVariable(PistonDebugTarget debugTarget, PistonDebugThread thread, String name, PistonDebugValue value)
    {
        super(debugTarget);
        
        this.debugTarget = debugTarget;
        this.thread = thread;
        this.name = name;
        this.value = value;
    }

    @Override
    public String getName() throws DebugException
    {
        return name;
    }

    @Override
    public String getReferenceTypeName() throws DebugException
    {
        return value.getReferenceTypeName();
    }

    @Override
    public IValue getValue() throws DebugException
    {
        return value;
    }

    @Override
    public boolean hasValueChanged() throws DebugException
    {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public String getModelIdentifier()
    {
        return thread.getModelIdentifier();
    }

    @Override
    public void setValue(String expression) throws DebugException
    {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setValue(IValue value) throws DebugException
    {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean supportsValueModification()
    {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean verifyValue(String expression) throws DebugException
    {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean verifyValue(IValue value) throws DebugException
    {
        // TODO Auto-generated method stub
        return false;
    }
}