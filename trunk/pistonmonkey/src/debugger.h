#ifndef PISTONMONKEY_DEBUGGER_H_
#define PISTONMONKEY_DEBUGGER_H_

extern char *					gDebuggerLocalAddress;
extern int						gDebuggerLocalPort;


struct activeScript
{
	JSScript * 	script;
	JSContext * cx;
	char *		fullPath;
	ino_t		inodeNum;
	unsigned int startLine;
	unsigned int endLine;
};

struct activeContext
{
	JSContext * cx;
	char *		name;
	int			num;
	bool		suspended;
};

struct breakpoint
{
	char *		fullPath;
	ino_t		inodeNum;
	unsigned int			line;
};

typedef enum DebuggerStepType {
    STEP_NONE,
    STEP_OVER,
    STEP_INTO,
    STEP_RETURN
} DebuggerStepType;

struct debugStep
{
	DebuggerStepType	type;
	struct activeScript * 			as;
	struct activeContext * 		ac;
	unsigned int					lastStopLine;
};

void 			debugger_init(const char * address, int port);
void 			debugger_main(void * arg);
void 			debugger_shutdown();
void 			debugger_context_created(JSContext * cx, const char * name);
void 			debugger_context_destroyed(JSContext * cx);
struct activeContext * debugger_context_getActiveContext(JSContext * cx);
struct activeContext * debugger_context_getActiveContextByNum(int num);
void 			debugger_script_add(JSContext * cx, JSScript * script, const char * filename, unsigned int line);
void 			debugger_script_remove(JSScript * script);
struct activeScript * debugger_script_getActiveScript(JSScript * script);
void 			debugger_new_script_handler(JSContext * cx, const char * name, uintN line, JSScript * script, JSFunction * fun, void * arg);
void 			debugger_destroy_script_handler(JSContext * cx, JSScript * script, void * arg);
void * 			debugger_execute_hook(JSContext * cx, JSStackFrame * fp, JSBool before, JSBool * ok, void * arg);
JSTrapStatus 	debugger_breakpoint_handler(JSContext * cx, JSScript * script, jsbytecode * pc, jsval * rval, void * opaque);
JSTrapStatus 	debugger_step_handler(JSContext * cx, JSScript * script, jsbytecode * pc, jsval * rval, void * arg);
void 			debugger_webinterface_handler(struct evhttp_request * req, void * arg);
void 			debugger_send_message(const char * messageName, const char * messageData);
bool			debugger_received_message(const char * messageName, struct evbuffer * messageData);
struct breakpoint * debugger_breakpoint_create(char * filename, unsigned int line);
void 				debugger_breakpoint_set(struct breakpoint * bp);

#endif /* PISTONMONKEY_DEBUGGER_H_ */

