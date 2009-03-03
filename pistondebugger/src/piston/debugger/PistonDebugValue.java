package piston.debugger;

import org.eclipse.debug.core.DebugException;
import org.eclipse.debug.core.model.DebugElement;
import org.eclipse.debug.core.model.IValue;
import org.eclipse.debug.core.model.IVariable;


public class PistonDebugValue extends DebugElement implements IValue
{
    protected PistonDebugTarget debugTarget;
    protected PistonDebugThread thread;
    protected String type;
    protected String value;
    protected boolean hasChildren=false;
    
    public PistonDebugValue(PistonDebugTarget debugTarget, PistonDebugThread thread, String type, String value, boolean hasChildren)
    {
        super(debugTarget);
        
        this.debugTarget = debugTarget;
        this.thread = thread;
        this.type = type;
        this.value = value;
        this.hasChildren = hasChildren;
    }

    @Override
    public String getReferenceTypeName() throws DebugException
    {
        return type;
    }

    @Override
    public String getValueString() throws DebugException
    {
        return value;
    }

    @Override
    public IVariable[] getVariables() throws DebugException
    {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public boolean hasVariables() throws DebugException
    {
        return hasChildren;
    }

    @Override
    public boolean isAllocated() throws DebugException
    {
        return true;
    }

    @Override
    public String getModelIdentifier()
    {
        return thread.getModelIdentifier();
    }    
}