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
#include "Directory.h"

JSBool Directory_function_exists_internal(JSContext * cx, char * path, jsval * rval)
{
	struct stat statInfo;

	int result = stat(path, &statInfo);
	if(result==-1)
		*rval = BOOLEAN_TO_JSVAL(false);
	else
		*rval = BOOLEAN_TO_JSVAL(true);

	return JS_TRUE;
}

LM_FUNCTION(Directory_function_exists_static, "static Directory.exists")
	LM_ASSERT_ARG_COUNT(1)
	LM_ASSERT_JSVAL_STRING(argv[0], "argument 'path'")

	char * path = LM_JSVAL_TO_CHAR(argv[0]);
	LM_ASSERT(path!=0 && strlen(path)>0, " required argument 'path' is empty")

	return Directory_function_exists_internal(cx, path, rval);
}

LM_FUNCTION(Directory_function_exists, "Directory.exists")
	LM_ASSERT_ARG_COUNT(0)

	LM_GET_PROPERTY("path", pathVal)
	LM_ASSERT_JSVAL_STRING(pathVal, "property 'path'")

	char * path = LM_JSVAL_TO_CHAR(pathVal);
	LM_ASSERT(path!=0 && strlen(path)>0, " required property 'path' is empty")

	return Directory_function_exists_internal(cx, path, rval);
}
