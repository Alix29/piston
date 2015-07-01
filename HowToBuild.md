**WARNING**
This stuff isn't polished for wide public consumption. So there are no easy configure scripts. Sorry.

**REQUIREMENTS**
  * You must have Eclipse 3.4.1
  * You must also be running Java 6 or higher
  * You must have Aptana installed (only used for handling UI syntax highlighting, jumping to lines in the editor and placing breakpoints visually. some day I may write my own stuff here... maybe)

**DETAILS**
  * Open Eclipse, create a new workspace.
  * Go to Window->Open Perspective->Other (Choose SVN Repository Exporing)
  * Add a new repository: http://piston.googlecode.com/svn/trunk/
  * Browse to trunk and find the four folders:
    * pistonmonkey - C++ app that embeds tracemonkey and supports being debugged
    * pistondebugger - The actual eclipse plugin code for debugging javascript in eclipse
    * pistondocs - Piston's available JS API's are documented here. See pub/index.html
    * sandbox - A collection of assorted stuff
  * Select these four folders, right click and choose 'Check Out'

Each of these folders is an eclipse project.

**pistonmonkey**
  * To build this you'll need the 'Eclipse C/C++ Development Tools' and Platform plugins installed
    * Menu: Help->Software Updates, go to the 'Available Software' tab
    * Expand to and check this: Ganymede Update Site->C and C++ Development
    * Click 'Install...' on the right. Install any needed pre-reqs, restart eclipse.
  * Window->Open Perspective->Other (C/C++)
  * At this point you probably have a large number of build errors in the 'Problems' view (Window->Show View->Problems)
  * Locate pistonmonkey/libs folder
    * Notice you have four sub directories, curl, libevent, nspr, tracemonkey
    * Each one of these needs a 'debug' directory in it
    * This directory should point to the location where each of the libs libs and include files can be found
    * These may be installed on your system (/usr/local or /usr/lib) or you can download and build them yourself in any directory you want (This is what I do)
    * To build them yourself, follow the steps: http://piston.googlecode.com/svn/trunk/sandbox/libs_info.txt
  * Once you get the 'debug' folders pointed to the right place for each of the needed items, doing a 'Project->Clean' should result in a build with zero errors. Woot!
  * You can now run pistonmonkey or debug javascript with it with the debugger.

**pistondebugger**

First option. Take the pistondebugger/out/plugins/pistondebugger\_1.0.0.jar and put it in your eclipse/dropins/ directory.

Again, put that jar into the 'dropins' folder of where your eclipse application is.

Second option, run from the source (allows debugging):
  * To 'run' the piston debugger (an eclipse plugin) you'll need to have the 'Eclipse Plugin-in Development Environment' installed in your Eclipse.
    * Menu: Help->Software Updates, go to the 'Available Software' tab
    * Expand to and check this: Ganymede Update Site->Java Development->Eclipse Plugin-in Development Environment
    * Click 'Install...' on the right. Install any needed pre-reqs, restart eclipse.
  * Window->Open Perspective->Other (Plugin-in Development)
  * Select the pistondebugger project
  * With any luck this project won't have an build errors in the 'Problems' view (Window->Show View->Problems)
  * Window->Run Configurations...
  * Select the 'Piston Debugger' under the 'Eclipse Application' category. Hit Run
  * With luck another eclipse will start. And now you can run the debugger. See the Overview on DebuggerProtocolDetails