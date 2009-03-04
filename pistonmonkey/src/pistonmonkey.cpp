// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <list>

using namespace std;

// lib includes
#include <jsapi.h>
#include <jsdbgapi.h>
#include <curl.h>

// local includes
#include "pistonmonkey.h"
#include "signal.h"
#include "command_line.h"
#include "debugger.h"
#include "js.h"
#include "jsutil.h"
#include "modules/File/File.h"
#include "modules/Piston.h"

// globals
bool gDebuggerEnabled = false;
bool gWaitingForDebugger = false;
bool gRunning = true;


//////////////////////////////////////////////////////////////////////////
// main
int main(int argc, char ** argv)
{
    signal_init();
    char * filename = command_line_handle(argc, argv);

    curl_global_init(CURL_GLOBAL_ALL);		// This has to go here outside of any and all threads

    Piston_preinit();

    js_init();

    if(gDebuggerEnabled)
    	debugger_init();

    while(gDebuggerEnabled && gWaitingForDebugger && gRunning)
    {
    	usleep(100000);
    }

    if(gRunning)
    {
		// Run our script
		JSContext * cx = js_create_context("main");
		if(cx)
		{
			JS_BeginRequest(cx);
			jsval executeResult = js_run_script(filename, cx, gJSGlobal);

			// This should not always happen but either never happen or be optional
			printf("%s\n", JS_GetStringBytes(JS_ValueToString(cx, executeResult)));
			fflush(stdout);

			// MAY NEED TO JOIN THREADS HERE IF OTHER THREADS ARE OPEN

			JS_EndRequest(cx);
			js_destroy_context(cx);
		}
    }

    if(gDebuggerEnabled)
    	debugger_shutdown();

    gRunning = false;

    js_shutdown();

    return 0;
}
