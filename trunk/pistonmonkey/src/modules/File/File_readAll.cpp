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

//////////////////////////////////////////////////////////////////////////
// readAll_internal
// No primitve because we read directly into JS allocated memory
JSBool File_function_readAll_internal(JSContext * cx, char * path, FILE * fp, jsval * rval)
{
	if(!fp)
		return JS_FALSE;

	off_t size;
	if(!File_function_size_primitive(path, &size))
	{
		char error_buf[MAX_ERROR_LENGTH];
		JS_ReportError(cx, "File.readAll failed to stat file [%s] Error (%d): %s", path, errno, strerror_r(errno, error_buf, MAX_ERROR_LENGTH));
		return JS_FALSE;
	}

	char * data = (char *)JS_malloc(cx, size+1);
	if(!data)
	{
		JS_ReportError(cx, "File.readAll unable to allocate %d bytes of memory", size);
		return JS_FALSE;
	}
	memset(data, 0, size+1);

	size_t result = fread(data, 1, size, fp);
	if(result!=(size_t)size)
	{
		JS_free(cx, data);

		if(ferror(fp))
		{
			char error_buf[MAX_ERROR_LENGTH];
			JS_ReportError(cx, "File.readAll encountered error attempting to read %d bytes from file [%s] into memory Error (%d): %s", size, path, errno, strerror_r(errno, error_buf, MAX_ERROR_LENGTH));
			return JS_FALSE;
		}

		JS_ReportError(cx, "File.readAll attempted to read %d bytes but could only read %d", size, result);
		return JS_FALSE;
	}

	*rval = STRING_TO_JSVAL(JS_NewString(cx, data, result));

	return JS_TRUE;
}

LM_FUNCTION(File_function_readAll_static, "static File.readAll")
	LM_ASSERT_ARG_COUNT(1)
	LM_ASSERT_JSVAL_STRING(argv[0], "argument 'path'")

	char * path = LM_JSVAL_TO_CHAR(argv[0]);
	LM_ASSERT(path!=0 && strlen(path)>0, " required argument 'path' is empty")

	FILE * fp = fopen(path, "r");
	if(!fp)
	{
		char error_buf[MAX_ERROR_LENGTH];
		JS_ReportError(cx, "File.readAll failed to open [%s] Error (%d): %s", path, errno, strerror_r(errno, error_buf, MAX_ERROR_LENGTH));
		return JS_FALSE;
	}

	JSBool result = File_function_readAll_internal(cx, path, fp, rval);

	fclose(fp);

	return result;
}

LM_FUNCTION(File_function_readAll, "File.readAll")
	LM_ASSERT_ARG_COUNT(0)

	bool hadToOpen = false;

	LM_GET_PROPERTY("path", pathVal)
	LM_ASSERT_JSVAL_STRING(pathVal, "property 'path'")

	File_data * data = (File_data *)JS_GetPrivate(cx, obj);

	char * path = LM_JSVAL_TO_CHAR(pathVal);
	LM_ASSERT(path!=0 && strlen(path)>0, " required property 'path' is empty")

	if(!data->fp)
	{
		data->fp = fopen(path, "r");

		if(!data->fp)
		{
			char error_buf[MAX_ERROR_LENGTH];
			JS_ReportError(cx, "File.readAll failed to open [%s] Error (%d): %s", path, errno, strerror_r(errno, error_buf, MAX_ERROR_LENGTH));
		}
		else
			hadToOpen = true;
	}

	JSBool result = File_function_readAll_internal(cx, path, data->fp, rval);;

	if(hadToOpen)
	{
		fclose(data->fp);
		data->fp = 0;
	}

	return result;
}
