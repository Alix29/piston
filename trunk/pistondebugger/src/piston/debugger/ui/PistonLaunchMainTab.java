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
    private Button launchAppCheckbox; 
    private Button appBrowseButton;
    private Text appText;
    private Text remoteHostText;
    private Text remotePortText;
    private Text localHostText;
    private Text localPortText;
    
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
        createRemoteHostSection(composite);
        createRemotePortSection(composite);
        createLocalHostSection(composite);
        createLocalPortSection(composite);
        createJSFileSection(composite);

        // hook up event handlers to update the configuration dialog when settings change
        hookListeners(true);
        
    }
    
    private void createAppSection(Composite parent)
    {
        Font font = parent.getFont();

        Group group = new Group(parent, SWT.FLAT | SWT.SHADOW_IN);
        group.setText("Piston Application");
        group.setFont(font);
        group.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
        GridLayout groupLayout = new GridLayout();
        groupLayout.numColumns = 2;
        groupLayout.makeColumnsEqualWidth = false;
        group.setLayout(groupLayout);
        
        launchAppCheckbox = new Button(group, SWT.CHECK);
        launchAppCheckbox.setText("Launch local piston application");
        launchAppCheckbox.setSelection(true);
        launchAppCheckbox.redraw();
        launchAppCheckbox.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false, 2, 1));

        appText = new Text(group, SWT.SINGLE | SWT.BORDER);
        appText.setLayoutData(new GridData(SWT.FILL, SWT.BOTTOM, true, false));

        appBrowseButton = new Button(group, SWT.PUSH);
        appBrowseButton.setText("Browse...");
        appBrowseButton.setLayoutData(new GridData(SWT.RIGHT, SWT.BOTTOM, false, false));

        appBrowseButton.addSelectionListener(new SelectionAdapter()
        {
            public void widgetSelected(SelectionEvent e)
            {
                String appPath = chooseFilesystemLocation();
                if(appPath!=null)
                    appText.setText(appPath);
            }
        });
                
        launchAppCheckbox.addSelectionListener(new SelectionAdapter()
        {
            public void widgetSelected(SelectionEvent e)
            {
                handleLaunchCheckboxChange();
                setDirty(true);
                updateLaunchConfigurationDialog();    
            }
        });
    }
    
    private void handleLaunchCheckboxChange()
    {
        if(launchAppCheckbox.getSelection())
        {
            appText.setEnabled(true);
            appBrowseButton.setEnabled(true);
        }
        else
        {
            appText.setEnabled(false);
            appBrowseButton.setEnabled(false);
        }
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
    
    private void createRemoteHostSection(Composite parent)
    {
        Font font = parent.getFont();

        Group group = new Group(parent, SWT.FLAT | SWT.SHADOW_IN);
        group.setText("Piston Application Debug Host");
        group.setFont(font);
        group.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
        GridLayout groupLayout = new GridLayout();
        groupLayout.numColumns = 2;
        groupLayout.makeColumnsEqualWidth = false;
        group.setLayout(groupLayout);

        remoteHostText = new Text(group, SWT.SINGLE | SWT.BORDER);
        remoteHostText.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
    }
    
    private void createRemotePortSection(Composite parent)
    {
        Font font = parent.getFont();

        Group group = new Group(parent, SWT.FLAT | SWT.SHADOW_IN);
        group.setText("Piston Application Debug Port");
        group.setFont(font);
        group.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
        GridLayout groupLayout = new GridLayout();
        groupLayout.numColumns = 2;
        groupLayout.makeColumnsEqualWidth = false;
        group.setLayout(groupLayout);

        remotePortText = new Text(group, SWT.SINGLE | SWT.BORDER);
        remotePortText.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
    }
    
    private void createLocalHostSection(Composite parent)
    {
        Font font = parent.getFont();

        Group group = new Group(parent, SWT.FLAT | SWT.SHADOW_IN);
        group.setText("Debugger Host");
        group.setFont(font);
        group.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
        GridLayout groupLayout = new GridLayout();
        groupLayout.numColumns = 2;
        groupLayout.makeColumnsEqualWidth = false;
        group.setLayout(groupLayout);

        localHostText = new Text(group, SWT.SINGLE | SWT.BORDER);
        localHostText.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
    }
    
    private void createLocalPortSection(Composite parent)
    {
        Font font = parent.getFont();

        Group group = new Group(parent, SWT.FLAT | SWT.SHADOW_IN);
        group.setText("Debugger Port");
        group.setFont(font);
        group.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
        GridLayout groupLayout = new GridLayout();
        groupLayout.numColumns = 2;
        groupLayout.makeColumnsEqualWidth = false;
        group.setLayout(groupLayout);

        localPortText = new Text(group, SWT.SINGLE | SWT.BORDER);
        localPortText.setLayoutData(new GridData(SWT.FILL, SWT.TOP, true, false));
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
            appText.addListener(SWT.Modify, dirtyListener);
            jsFileText.addListener(SWT.Modify, dirtyListener);
            remoteHostText.addListener(SWT.Modify, dirtyListener);
            remotePortText.addListener(SWT.Modify, dirtyListener);
            localHostText.addListener(SWT.Modify, dirtyListener);
            localPortText.addListener(SWT.Modify, dirtyListener);
        }
        else
        {
            appText.removeListener(SWT.Modify, dirtyListener);
            jsFileText.removeListener(SWT.Modify, dirtyListener);
            remoteHostText.removeListener(SWT.Modify, dirtyListener);
            remotePortText.removeListener(SWT.Modify, dirtyListener);
            localHostText.removeListener(SWT.Modify, dirtyListener);
            localPortText.removeListener(SWT.Modify, dirtyListener);
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
            remoteHostText.setText(configuration.getAttribute("remoteHost", "localhost"));
            remotePortText.setText(configuration.getAttribute("remotePort", "7570"));
            localHostText.setText(configuration.getAttribute("localHost", "localhost"));
            localPortText.setText(configuration.getAttribute("localPort", "7580"));
            launchAppCheckbox.setSelection(Boolean.parseBoolean(configuration.getAttribute("launchApp", "true")));
            launchAppCheckbox.redraw();
            handleLaunchCheckboxChange();
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
        configuration.setAttribute("remoteHost", remoteHostText.getText());
        configuration.setAttribute("remotePort", remotePortText.getText());
        configuration.setAttribute("localHost", localHostText.getText());
        configuration.setAttribute("localPort", localPortText.getText());
        configuration.setAttribute("launchApp", Boolean.toString(launchAppCheckbox.getSelection()));
    }

    @Override
    public void setDefaults(ILaunchConfigurationWorkingCopy configuration)
    {
    }
    
}