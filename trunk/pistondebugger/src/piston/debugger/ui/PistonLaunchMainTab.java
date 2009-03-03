package piston.debugger.ui;

import java.io.File;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.ui.AbstractLaunchConfigurationTab;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;

import piston.debugger.PistonActivator;

public class PistonLaunchMainTab extends AbstractLaunchConfigurationTab
{
    private Listener dirtyListener;
    private Button jsFileBrowseButton;
    private Text jsFileText;
    private Button appBrowseButton;
    private Text appText;
    
    @Override
    public void createControl(Composite parent)
    {
        Font font = parent.getFont();

        Composite composite = new Composite(parent, SWT.NONE);
        setControl(composite);
        GridLayout topLayout = new GridLayout();
        topLayout.verticalSpacing = 5;
        composite.setLayout(topLayout);
        composite.setFont(font);

        dirtyListener = new Listener()
        {
            public void handleEvent(Event event)
            {
                setDirty(true);
                updateLaunchConfigurationDialog();
            }
        };

        createAppSection(composite);
        createJSFileSection(composite);

        // hook up event handlers to update the configuration dialog when settings change
        hookListeners(true);
        
    }
    
    private void createAppSection(Composite parent)
    {
        Font font = parent.getFont();

        Group group = new Group(parent, SWT.FLAT | SWT.SHADOW_IN);
        group.setText("App");
        group.setFont(font);
        group.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
        GridLayout groupLayout = new GridLayout();
        groupLayout.numColumns = 2;
        groupLayout.makeColumnsEqualWidth = false;
        group.setLayout(groupLayout);
        
        appText = new Text(group, SWT.SINGLE | SWT.BORDER);
        appText.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));

        appBrowseButton = new Button(group, SWT.PUSH);
        appBrowseButton.setText("Browse...");
        appBrowseButton.setLayoutData(new GridData(SWT.RIGHT, SWT.TOP, false, false));

        appBrowseButton.addSelectionListener(new SelectionAdapter()
        {
            public void widgetSelected(SelectionEvent e)
            {
                String appPath = chooseFilesystemLocation();
                if(appPath!=null)
                    appText.setText(appPath);
            }
        });
    }
        
    private void createJSFileSection(Composite parent)
    {
        Font font = parent.getFont();

        Group group = new Group(parent, SWT.FLAT | SWT.SHADOW_IN);
        group.setText("JavaScript File");
        group.setFont(font);
        group.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
        GridLayout groupLayout = new GridLayout();
        groupLayout.numColumns = 2;
        groupLayout.makeColumnsEqualWidth = false;
        group.setLayout(groupLayout);

        jsFileText = new Text(group, SWT.SINGLE | SWT.BORDER);
        jsFileText.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));

        jsFileBrowseButton = new Button(group, SWT.PUSH);
        jsFileBrowseButton.setText("Browse...");
        jsFileBrowseButton.setLayoutData(new GridData(SWT.RIGHT, SWT.TOP, false, false));

        jsFileBrowseButton.addSelectionListener(new SelectionAdapter()
        {
            public void widgetSelected(SelectionEvent e)
            {
                String jsFilePath = chooseFilesystemLocation();
                if(jsFilePath != null)
                    jsFileText.setText(jsFilePath);
            }
        });
    }
    
    private String chooseFilesystemLocation()
    {
        FileDialog dialog = new FileDialog(getShell(), SWT.OPEN);
        dialog.open();
        String fileName = dialog.getFileName();
        if(fileName==null || fileName.length()<1)
            return null;
        
        return dialog.getFilterPath() + File.separator + dialog.getFileName();
    }
    
    private void hookListeners(boolean hook)
    {
        if (hook)
        {
            jsFileText.addListener(SWT.Modify, dirtyListener);
        }
        else
        {
            jsFileText.removeListener(SWT.Modify, dirtyListener);
        }
    }

    @Override
    public String getName()
    {
        return "Main";
    }

    @Override
    public void initializeFrom(ILaunchConfiguration configuration)
    {
        hookListeners(false);
        try
        {
            appText.setText(configuration.getAttribute("appPath", ""));
            jsFileText.setText(configuration.getAttribute("jsFilePath", ""));
        }
        catch (CoreException e)
        {
            Status logStatus = new Status(IStatus.ERROR, PistonActivator.getDefault().getBundle().getSymbolicName(), IStatus.OK, "Reading launch configuration fails", e);
            PistonActivator.getDefault().getLog().log(logStatus);
        }
        finally
        {
            hookListeners(true);
        }
    }

    @Override
    public void performApply(ILaunchConfigurationWorkingCopy configuration)
    {
        configuration.setAttribute("appPath", appText.getText());
        configuration.setAttribute("jsFilePath", jsFileText.getText());
    }

    @Override
    public void setDefaults(ILaunchConfigurationWorkingCopy configuration)
    {
    }
    
}