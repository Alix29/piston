// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <list>

using namespace std;

// lib includes
#include <jsapi.h>
#include <jsdbgapi.h>

// local includes
#include "pistonmonkey.h"
#include "js.h"
#include "debugger.h"
#include "modules/Piston.h"


// wide globals
JSRuntime *		gJSRuntime=0;
JSObject *   	gJSGlobal=0;

// local globals
static JSClass 	js_global_class =
	{ "global", JSCLASS_GLOBAL_FLAGS,
	  JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
	  JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	  JSCLASS_NO_OPTIONAL_MEMBERS };


//////////////////////////////////////////////////////////////////////////
// js_init
void js_init()
{
	gJSRuntime = JS_NewRuntime(8L * 1024L * 1024L);
    if(gJSRuntime==0)
        exit(1);
}


//////////////////////////////////////////////////////////////////////////
// js_shutdown
void js_shutdown()
{
    JS_DestroyRuntime(gJSRuntime);
    JS_ShutDown();
}


//////////////////////////////////////////////////////////////////////////
// js_create_context
JSContext * js_create_context(const char * name)
{
	JSContext * cx = JS_NewContext(gJSRuntime, 8192);
	if(cx==0)
		return 0;

	JS_BeginRequest(cx);

	JS_SetOptions(cx, JSOPTION_VAROBJFIX);
	JS_SetVersion(cx, JSVERSION_LATEST);
	JS_SetErrorReporter(cx, js_error_handler);
	JS_SetOperationCallback(cx, js_operation_callback);

	if(!gJSGlobal)
	{
		gJSGlobal = JS_NewObject(cx, &js_global_class, 0, 0);
		if(!gJSGlobal)
		{
			JS_EndRequest(cx);
			return 0;
		}

		if(!JS_InitStandardClasses(cx, gJSGlobal))
		{
			JS_EndRequest(cx);
			return 0;
		}

		Piston_init(cx, gJSGlobal);
	}
	else
	{
		JS_SetGlobalObject(cx, gJSGlobal);
	}

	if(gDebuggerEnabled)
		debugger_context_created(cx, name);

	JS_EndRequest(cx);

	return cx;
}


//////////////////////////////////////////////////////////////////////////
// js_destroy_context
void js_destroy_context(JSContext * cx)
{
	if(gDebuggerEnabled)
		debugger_context_destroyed(cx);

	JS_DestroyContext(cx);
}


//////////////////////////////////////////////////////////////////////////
// js_run_script
jsval js_run_script(char * filename, JSContext * cx, JSObject * scopeObject)
{
	jsval executeResult = JSVAL_VOID;

	if(!filename)
		return executeResult;

    JS_BeginRequest(cx);

    // Compile script
	JSScript * script = JS_CompileFile(cx, scopeObject, filename);
	if(script)
	{
		// Create object based on it so we can root it
		JSObject * scriptObject = JS_NewScriptObject(cx, script);
		if(scriptObject && JS_AddRoot(cx, &scriptObject))
		{
			// Execute it
			JS_ExecuteScript(cx, scopeObject, script, &executeResult);

			// Remove rooting, allowing GC to take place
			JS_RemoveRoot(cx, &scriptObject);
		}
	}

    JS_EndRequest(cx);

    return executeResult;
}


//////////////////////////////////////////////////////////////////////////
// js_operation_callback
JSBool js_operation_callback(JSContext * cx)
{
	JS_MaybeGC(cx);

	return JS_TRUE;
}


//////////////////////////////////////////////////////////////////////////
// js_error_handler
void js_error_handler(JSContext * cx, const char * message, JSErrorReport * report)
{
    fprintf(stderr, "%s:%u %s\n",
            report->filename ? report->filename : "<no filename>",
            (unsigned int) report->lineno,
            message);
}
