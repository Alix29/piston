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

bool File_function_size_primitive(char * path, off_t * size)
{
	struct stat statInfo;

	int result = stat(path, &statInfo);
	if(result==-1)
		return false;

	*size = statInfo.st_size;

	return true;
}

JSBool File_function_size_internal(JSContext * cx, char * path, jsval * rval)
{
	off_t 	size;
	if(!File_function_size_primitive(path, &size))
	{
		char error_buf[MAX_ERROR_LENGTH];
		JS_ReportError(cx, "File.size failed to open [%s] Error (%d): %s", path, errno, strerror_r(errno, error_buf, MAX_ERROR_LENGTH));
		return JS_FALSE;
	}

	*rval = INT_TO_JSVAL(size);

	return JS_TRUE;
}

LM_FUNCTION(File_function_size_static, "static File.size")
	LM_ASSERT_ARG_COUNT(1)
	LM_ASSERT_JSVAL_STRING(argv[0], "argument 'path'")

	char * path = LM_JSVAL_TO_CHAR(argv[0]);
	LM_ASSERT(path!=0 && strlen(path)>0, " required argument 'path' is empty")

	return File_function_size_internal(cx, path, rval);
}

LM_FUNCTION(File_function_size, "File.size")
	LM_ASSERT_ARG_COUNT(0)

	LM_GET_PROPERTY("path", pathVal)
	LM_ASSERT_JSVAL_STRING(pathVal, "property 'path'")

	char * path = LM_JSVAL_TO_CHAR(pathVal);
	LM_ASSERT(path!=0 && strlen(path)>0, " required property 'path' is empty")

	return File_function_size_internal(cx, path, rval);
}
