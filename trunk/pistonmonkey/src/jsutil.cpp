// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include <cstring>
#include <vector>

// lib includes
#include <jsapi.h>

// local includes
#include "pistonmonkey.h"
#include "jsutil.h"
#include "memutil.h"


//////////////////////////////////////////////////////////////////////////
// JS_CopyObjectProperties
JSBool JS_CopyObjectProperties(JSContext * cx, JSObject * src, JSObject * dst)
{
	JSIdArray * ida = JS_Enumerate(cx, src);
	if(ida)
	{
		jsval keyVal;
		jsval valueVal;

		for(int i=0;i<ida->length;i++)
		{
			JS_IdToValue(cx, ida->vector[i], &keyVal);
			char * key = JS_GetStringBytes(JS_ValueToString(cx, keyVal));

			JS_GetProperty(cx, src, key, &valueVal);
			if(!JS_SetProperty(cx, dst, key, &valueVal))
				return JS_FALSE;
		}
	}

	return JS_TRUE;
}


//////////////////////////////////////////////////////////////////////////
// JS_ValueType
const char * JS_ValueType(JSContext * cx, jsval * val)
{
	if(JSVAL_IS_BOOLEAN(*val))
		return "Boolean";
	if(JSVAL_IS_INT(*val))
		return "Integer";
	if(JSVAL_IS_DOUBLE(*val) || JSVAL_IS_NUMBER(*val))
		return "Double";
	if(JSVAL_IS_NULL(*val))
		return "Null";
	if(JSVAL_IS_VOID(*val))
		return "void";
	if(JSVAL_IS_STRING(*val))
		return "String";
	if(JSVAL_IS_OBJECT(*val))
		return "Object";

	return "unknown";
}
