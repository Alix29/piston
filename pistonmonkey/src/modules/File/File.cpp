// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

// lib includes
#include <jsapi.h>

// local includes
#include "../../pistonmonkey.h"
#include "../../jsutil.h"
#include "../../memutil.h"
#include "File.h"

// local globals
static JSFunctionSpec File_functions[] =
{
	JS_FS("exists", File_function_exists, 0, 0, 0),
	JS_FS("isFile", File_function_isFile, 0, 0, 0),
	JS_FS("isDirectory", File_function_isDirectory, 0, 0, 0),
	JS_FS("readAll", File_function_readAll, 0, 0, 0),
	JS_FS("size", File_function_size, 0, 0, 0),
	JS_FS_END
};

static JSFunctionSpec File_functions_static[] =
{
	JS_FS("exists", File_function_exists_static, 0, 0, 0),
	JS_FS("isFile", File_function_isFile_static, 0, 0, 0),
	JS_FS("isDirectory", File_function_isDirectory_static, 0, 0, 0),
	JS_FS("readAll", File_function_readAll_static, 0, 0, 0),
	JS_FS("size", File_function_size_static, 0, 0, 0),
	JS_FS_END
};

static JSPropertySpec File_propertes[] =
{
	JS_PS("path", 0, JSPROP_ENUMERATE|JSPROP_PERMANENT, 0, File_property_path_set),
	JS_PS_END
};

JSClass File_class =
{
	"File",					//         name: The class name
	JSCLASS_HAS_PRIVATE,	//        flags: Various flags
	JS_PropertyStub,		//  addProperty: A hook called just after adding a new property. May modify the new property value. (Default: JS_PropertyStub)
	JS_PropertyStub,		//  delProperty: A hook called when deleting a property. May veto. (Default: JS_PropertyStub)
	JS_PropertyStub,		//  getProperty: A hook called when getting a property. This is the default getter for the class. (Default: JS_PropertyStub)
	JS_PropertyStub,		//  setProperty: A hook called when setting a property. This is the default setter for the class.
							//               When a script creates a new property, this is called after addProperty. (Default: JS_PropertyStub)
	JS_EnumerateStub,		//    enumerate: Method for enumerating object properties (Default: JS_EnumerateStub)
	JS_ResolveStub,			//      resolve: Hook for implementing lazy properties. See JSClass.resolve for details (Default: JS_ResolveStub)
	JS_ConvertStub,			//      convert: Method for converting property values. (Default: JS_ConvertStub)
	File_deconstructor,		//     finalize: The object finalizer hook. This is like the Java finalize() method. (Default: JS_FinalizeStub)
	0,						// getObjectOps: Pointer to an optional hook that returns a JSObjectOps, a set of additional object hooks.
							//               SpiderMonkey calls this hook each time a new object of this JSClass is created.
	0,						//  checkAccess: Optional method for access checks. Implements JS_CheckAccess.
	0,						//         call: This is called when a script calls an object as though it were a function: obj().
							//               If objects of your class shouldn't be callable, use NULL. Most objects are not callable.
	File_constructor,		//    construct: Pointer to the constructor for the object that represents this class.
	0,						//    xdrObject: Implements object serialization. If you do not use JSXDR, set this value to NULL.
	0,						//  hasInstance: Pointer to an optional hasInstance method for this object.
	0,						//         mark: Hook for the mark phase of garbage collection. A class must implement this hook
							//				 if it has private data that contains references to JavaScript values. Otherwise, set to NULL.
	0						// reserveSlots: A hook, called on object creation allowing the object to request reserved slots.
};


//////////////////////////////////////////////////////////////////////////
// preinit
void File_preinit()
{

}


//////////////////////////////////////////////////////////////////////////
// init
void File_init(JSContext * cx, JSObject * obj)
{
	JS_InitClass(cx,			         //           cx: Context
				 obj,			         //          obj: The parent/containing object
				 0, 			         // parent_proto: Pointer to an object to be used as a prototype.
				 &File_class,	         //        clasp: Pointer to the class structure to initialize.
				 File_constructor,       //  constructor: The constructor for the class. Scope matches obj.
				 1,                      //        nargs: Number of arguments for the constructor
				 File_propertes,         //           ps: Pointer to an array of JSPropertySpecS terminated by null.
				 File_functions,         //           fs: Pointer to an array of JSFunctionSpecS terminated by null.
				 0,		                 //    static_ps: Pointer to an array of JSPropertySpecS terminated by null.
				 File_functions_static); //    static_fs: Pointer to an array of JSFunctionSpecS terminated by null.
}


//////////////////////////////////////////////////////////////////////////
// constructor
JSBool File_constructor(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)
{
	File_data * data = (File_data *)malloc(sizeof(File_data));
	memset(data, 0, sizeof(File_data));

	JSObject * propertyMap = 0;
	if(argc==1)	// File(path)   or  File(propertyMap)
	{
		if(!JSVAL_IS_STRING(argv[0]) && !JSVAL_IS_OBJECT(argv[0]))
		{
			JS_ReportError(cx, "File constructor argument #1 has an unexpected type. Expected string or object, got: %s", JS_GetTypeName(cx, JS_TypeOfValue(cx, argv[0])));
			return JS_FALSE;
		}

		if(JSVAL_IS_OBJECT(argv[0]))
			JS_ValueToObject(cx, argv[0], &propertyMap);
		else if(JSVAL_IS_STRING(argv[0]))
			JS_SetProperty(cx, obj, "path", &argv[0]);
	}
	else if(argc==2)	// File(path, propertyMap)
	{
		if(!JSVAL_IS_STRING(argv[0]))
		{
			JS_ReportError(cx, "File constructor argument #1 has an unexpected type. Expected string, got: %s", JS_GetTypeName(cx, JS_TypeOfValue(cx, argv[0])));
			return JS_FALSE;
		}
		else if(!JSVAL_IS_OBJECT(argv[1]))
		{
			JS_ReportError(cx, "File constructor argument #2 has an unexpected type. Expected object, got: %s", JS_GetTypeName(cx, JS_TypeOfValue(cx, argv[1])));
			return JS_FALSE;
		}

		JS_SetProperty(cx, obj, "path", &argv[0]);

		JS_ValueToObject(cx, argv[1], &propertyMap);
	}

	if(propertyMap && !JS_CopyObjectProperties(cx, propertyMap, obj))
		return JS_FALSE;

	JS_SetPrivate(cx, obj, data);

	return JS_TRUE;
}


//////////////////////////////////////////////////////////////////////////
// deconstructor
void File_deconstructor(JSContext * cx, JSObject * obj)
{
	File_data * data = (File_data *)JS_GetPrivate(cx, obj);
	if(data)
	{
		if(data->fp)
			fclose(data->fp);

		memfree(data);
	}
}


//////////////////////////////////////////////////////////////////////////
// property_path_set
LM_PROPERTY_SET_FUNCTION(File_property_path_set, "File")
	LM_ASSERT_JSVAL_STRING((*vp), "property 'path'")
	return JS_TRUE;
}
