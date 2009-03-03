#ifndef PISTONMONKEY_JS_H_
#define PISTONMONKEY_JS_H_

extern JSRuntime *	gJSRuntime;
extern JSObject *   gJSGlobal;

void		js_init();
void 		js_shutdown();
JSContext * js_create_context(const char * name);
void 		js_destroy_context(JSContext * cx);
jsval 		js_run_script(char * filename, JSContext * cx, JSObject * scopeObject);
JSBool 		js_operation_callback(JSContext * cx);
void 		js_error_handler(JSContext * cx, const char * message, JSErrorReport * report);

#endif /* PISTONMONKEY_JS_H_ */
