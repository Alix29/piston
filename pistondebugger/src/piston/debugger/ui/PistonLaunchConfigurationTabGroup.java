package piston.debugger.ui;

import org.eclipse.debug.ui.AbstractLaunchConfigurationTabGroup;
import org.eclipse.debug.ui.CommonTab;
import org.eclipse.debug.ui.ILaunchConfigurationDialog;
import org.eclipse.debug.ui.ILaunchConfigurationTab;

public class PistonLaunchConfigurationTabGroup extends AbstractLaunchConfigurationTabGroup
{
    public void createTabs(ILaunchConfigurationDialog dialog, String mode)
    {
        ILaunchConfigurationTab[] tabs;
        tabs = new ILaunchConfigurationTab[] {new PistonLaunchMainTab(), new CommonTab()};
        setTabs(tabs);
    }

}
