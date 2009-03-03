// platform includes
#include <stdio.h>
#include <stdlib.h>

// lib includes
#include <jsapi.h>

// local includes
#include "../pistonmonkey.h"
#include "../jsutil.h"
#include "../js.h"
#include "File/File.h"
#include "Directory/Directory.h"
#include "Piston.h"

static JSClass Piston_class =
	{ "Piston", 0,
	  JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
	  JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
	  JSCLASS_NO_OPTIONAL_MEMBERS };


//////////////////////////////////////////////////////////////////////////
// Piston_preinit
void		Piston_preinit()
{
	File_preinit();
	Directory_preinit();
}


//////////////////////////////////////////////////////////////////////////
// Piston_init
JSObject * 	Piston_init(JSContext * cx, JSObject * parentObject)
{
	JSObject * piston = JS_DefineObject(cx, parentObject, "Piston",  &Piston_class, 0, 0);
	if(piston==0)
		return 0;

	char buf[8192];

	sprintf(buf, "%sURL.js", JS_DIRECTORY_PATH);
	js_run_script(buf, cx, piston);

	File_init(cx, piston);
	Directory_init(cx, piston);

	return piston;
}


