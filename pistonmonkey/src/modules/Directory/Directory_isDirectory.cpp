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

JSBool Directory_function_isDirectory_internal(JSContext * cx, char * path, jsval * rval)
{
	struct stat statInfo;

	int result = stat(path, &statInfo);
	if(result==-1)
	{
		char error_buf[MAX_ERROR_LENGTH];
		JS_ReportError(cx, "Directory.isDirectory failed to stat path [%s] Error (%d): %s", path, errno, strerror_r(errno, error_buf, MAX_ERROR_LENGTH));
		return JS_FALSE;
	}

	if(S_ISDIR(statInfo.st_mode))
		*rval = BOOLEAN_TO_JSVAL(true);
	else
		*rval = BOOLEAN_TO_JSVAL(false);

	return JS_TRUE;
}

LM_FUNCTION(Directory_function_isDirectory_static, "static Directory.isDirectory")
	LM_ASSERT_ARG_COUNT(1)
	LM_ASSERT_JSVAL_STRING(argv[0], "argument 'path'")

	char * path = LM_JSVAL_TO_CHAR(argv[0]);
	LM_ASSERT(path!=0 && strlen(path)>0, " required argument 'path' is empty")

	return Directory_function_isDirectory_internal(cx, path, rval);
}

LM_FUNCTION(Directory_function_isDirectory, "Directory.isDirectory")
	LM_ASSERT_ARG_COUNT(0)

	LM_GET_PROPERTY("path", pathVal)
	LM_ASSERT_JSVAL_STRING(pathVal, "property 'path'")

	char * path = LM_JSVAL_TO_CHAR(pathVal);
	LM_ASSERT(path!=0 && strlen(path)>0, " required property 'path' is empty")

	return Directory_function_isDirectory_internal(cx, path, rval);
}
