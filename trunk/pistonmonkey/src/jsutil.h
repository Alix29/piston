#ifndef PISTONMONKEY_JS_UTIL_H_
#define PISTONMONKEY_JS_UTIL_H_

#define JS_PS(name,tinyid,flags,getter,setter)  {name, tinyid, flags, getter, setter}
#define JS_PS_END JS_FS(0,0,0,0,0)

JSBool 	JS_CopyObjectProperties(JSContext * cx, JSObject * src, JSObject * dst);
const char * JS_ValueType(JSContext * cx, jsval * val);


#define LM_JSVAL_TO_CHAR(JSVAL)\
	JS_GetStringBytes(JS_ValueToString(cx, JSVAL))

#define LM_PROPERTY_SET_FUNCTION(FUNCTION_NAME, FUNCTION_NAME_TEXT)\
	JSBool FUNCTION_NAME(JSContext * cx, JSObject * obj, jsval idval, jsval * vp)\
	{\
		const char functionNameText[] = FUNCTION_NAME_TEXT;

#define LM_PROPERTY_SET_FUNCTION_REF(FUNCTION_NAME)\
	JSBool FUNCTION_NAME(JSContext * cx, JSObject * obj, jsval idval, jsval * vp);

#define LM_FUNCTION_REF(FUNCTION_NAME)\
	JSBool FUNCTION_NAME(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval);

#define LM_FUNCTION(FUNCTION_NAME, FUNCTION_NAME_TEXT)\
	JSBool FUNCTION_NAME(JSContext * cx, JSObject * obj, uintN argc, jsval * argv, jsval * rval)\
	{\
		const char functionNameText[] = FUNCTION_NAME_TEXT;

#define LM_GET_PROPERTY(PROPERTY_NAME, PROPERTY_VARIABLE_NAME)\
	jsval PROPERTY_VARIABLE_NAME;\
	JS_GetProperty(cx, obj, PROPERTY_NAME, &PROPERTY_VARIABLE_NAME);

#define LM_ASSERT(CONDITION, ERROR_MSG)\
	if(!(CONDITION))\
	{\
		char errorBuf[1024];\
		sprintf(errorBuf, "%s%s", functionNameText, ERROR_MSG);\
		JS_ReportError(cx, errorBuf);\
		return JS_FALSE;\
	}\

#define LM_ASSERT_ARG_COUNT(ARG_COUNT)\
	if(argc!=ARG_COUNT)\
	{\
		char errorBuf[1024];\
		if(ARG_COUNT==0)\
			sprintf(errorBuf, "%s does not take any arguments", functionNameText);\
		else\
			sprintf(errorBuf, "%s requires %d argument%s", functionNameText, ARG_COUNT, ARG_COUNT==1 ? "" : "s");\
		JS_ReportError(cx, errorBuf);\
		return JS_FALSE;\
	}

#define LM_ASSERT_JSVAL_STRING(JSVAL, JSVAL_NAME)\
	if(JSVAL_IS_NULL(JSVAL) || JSVAL_IS_VOID(JSVAL))\
	{\
		char errorBuf[1024];\
		sprintf(errorBuf, "%s required %s is null or undefined", functionNameText, JSVAL_NAME);\
		JS_ReportError(cx, errorBuf);\
		return JS_FALSE;\
	}\
	if(!JSVAL_IS_STRING(JSVAL))\
	{\
		char errorBuf[1024];\
		sprintf(errorBuf, "%s required %s has an unexpected type. Expected string, got: %s", functionNameText, JSVAL_NAME, JS_GetTypeName(cx, JS_TypeOfValue(cx, JSVAL)));\
		JS_ReportError(cx, errorBuf);\
		return JS_FALSE;\
	}

#endif /* PISTONMONKEY_JS_UTIL_H_ */
