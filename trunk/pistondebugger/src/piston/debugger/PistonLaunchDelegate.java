package piston.debugger;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.model.LaunchConfigurationDelegate;

public class PistonLaunchDelegate extends LaunchConfigurationDelegate
{
	public void launch(ILaunchConfiguration configuration, String mode, ILaunch launch, IProgressMonitor monitor) throws CoreException
	{
	    PistonDebugTarget debugTarget = new PistonDebugTarget(launch, configuration);
	    debugTarget.start();
	    launch.addDebugTarget(debugTarget);
	}
	
	@Override
	public ILaunch getLaunch(ILaunchConfiguration configuration, String mode) throws CoreException
	{
        return null;
    }
}
	