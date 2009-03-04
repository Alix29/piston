// platform includes
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <list>

using namespace std;

// lib includes
#include <prthread.h>
#include <event.h>
#include <evhttp.h>
#include <jsapi.h>
#include <jsdbgapi.h>
#include <curl.h>

// local includes
#include "pistonmonkey.h"
#include "debugger.h"
#include "jsutil.h"
#include "js.h"
#include "base64util.h"

// globals
char *							gDebuggerLocalAddress=0;
int								gDebuggerLocalPort=7570;

// local globals
PRThread * 						debuggerThread=0;
struct event_base * 			debuggerEventBase=0;
list<struct activeScript *> 	debuggerActiveScripts;
list<struct activeContext *>	debuggerActiveContexts;
list<struct breakpoint *>		debuggerBreakpoints;
const char *					debuggerRemoteAddress;
int								debuggerRemotePort;
int								debuggerContextCounter=0;
JSStackFrame *					debuggerStopFrame=0;

//////////////////////////////////////////////////////////////////////////
// debugger_init
void debugger_init(const char * address, int port)
{
	debuggerRemoteAddress = strdup(address);
	debuggerRemotePort = port;

	debuggerThread = PR_CreateThread(PR_USER_THREAD, debugger_main, 0, PR_PRIORITY_NORMAL, PR_GLOBAL_THREAD, PR_JOINABLE_THREAD, 0);
}


//////////////////////////////////////////////////////////////////////////
// debugger_main
void debugger_main(void * arg)
{
	//debugger_breakpoint_create("/mnt/deck/DevLab/runtime-EclipseApplication/testing/test.js", 4);
	if(!gDebuggerLocalAddress)
		gDebuggerLocalAddress = strdup("localhost");

	JS_SetNewScriptHook(gJSRuntime, debugger_new_script_handler, 0);
	JS_SetDestroyScriptHook(gJSRuntime, debugger_destroy_script_handler, 0);

	debuggerEventBase = event_base_new();
	struct evhttp * httpd = evhttp_new(debuggerEventBase);
	evhttp_bind_socket(httpd, gDebuggerLocalAddress, gDebuggerLocalPort);

	evhttp_set_gencb(httpd, debugger_webinterface_handler, 0);

	event_base_loop(debuggerEventBase, 0);

	evhttp_free(httpd);

	event_base_free(debuggerEventBase);
	debuggerEventBase = 0;
}


//////////////////////////////////////////////////////////////////////////
// debugger_shutdown
void debugger_shutdown()
{
	struct timeval now;
	now.tv_sec = 0;
	now.tv_usec = 0;

	if(debuggerEventBase)
	{
		event_base_loopbreak(debuggerEventBase);

		char localURL[4096];
		sprintf(localURL, "http://%s:%d/shutdown", gDebuggerLocalAddress, gDebuggerLocalPort);

		// This is needed because the loop won't break until the NEXT event, so let's give it the next event
		CURL * curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, localURL);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	if(debuggerThread)
	{
		PR_JoinThread(debuggerThread);
		debuggerThread = 0;
	}
}


//////////////////////////////////////////////////////////////////////////
// debugger_context_started
void debugger_context_created(JSContext * cx, const char * name)
{
	struct activeContext * ac = (struct activeContext *)malloc(sizeof(struct activeContext));
	ac->cx = cx;
	ac->name = (name==0 ? strdup("") : strdup(name));
	ac->num = debuggerContextCounter++;
	ac->suspended = false;
	debuggerActiveContexts.push_front(ac);

	char buf[8192];
	sprintf(buf, "%s\n%d\n", ac->name, ac->num);
	debugger_send_message("thread_created", buf);
}


//////////////////////////////////////////////////////////////////////////
// debugger_context_destroyed
void debugger_context_destroyed(JSContext * cx)
{
	struct activeContext * ac = debugger_context_getActiveContext(cx);
	if(ac==0)
		return;

	debuggerActiveContexts.remove(ac);
	free(ac->name);
	free(ac);
}


//////////////////////////////////////////////////////////////////////////
// debugger_context_getActiveContext
struct activeContext * debugger_context_getActiveContext(JSContext * cx)
{
	list<struct activeContext *>::iterator it;
	for(it=debuggerActiveContexts.begin();it!=debuggerActiveContexts.end();it++)
	{
		if((*it)->cx==cx)
			return (*it);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// debugger_context_getActiveContextByNum
struct activeContext * debugger_context_getActiveContextByNum(int num)
{
	list<struct activeContext *>::iterator it;
	for(it=debuggerActiveContexts.begin();it!=debuggerActiveContexts.end();it++)
	{
		if((*it)->num==num)
			return (*it);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// debugger_script_added
void debugger_script_add(JSContext * cx, JSScript * script, const char * filename, unsigned int line)
{
	struct activeScript * as = (struct activeScript *)malloc(sizeof(struct activeScript));
	as->script = script;
	as->cx = cx;
	as->startLine = line;
	as->endLine = (line + JS_GetScriptLineExtent(cx, script)) - 1;
	as->fullPath = strdup(filename);

	printf("Adding script %s lines %d to %d\n", filename, as->startLine, as->endLine);

	struct stat fileStat;
	stat(filename, &fileStat);
	as->inodeNum = fileStat.st_ino;

	debuggerActiveScripts.push_front(as);

	// Set any matching breakpoints we have for this filename
	list<struct breakpoint *>::iterator it;
	for(it=debuggerBreakpoints.begin();it!=debuggerBreakpoints.end();it++)
	{
		debugger_breakpoint_set((*it));
	}
}


//////////////////////////////////////////////////////////////////////////
// debugger_new_script_handler
void debugger_new_script_handler(JSContext * cx, const char * name, uintN line, JSScript * script, JSFunction * fun, void * arg)
{
	debugger_script_add(cx, script, name, line);
}


//////////////////////////////////////////////////////////////////////////
// debugger_script_remove
void debugger_script_remove(JSScript * script)
{
	struct activeScript * as = debugger_script_getActiveScript(script);
	if(as==0)
		return;

	debuggerActiveScripts.remove(as);
	free(as->fullPath);
	free(as);

	// Clear trap?
}


//////////////////////////////////////////////////////////////////////////
// debugger_destroy_script_handler
void debugger_destroy_script_handler(JSContext * cx, JSScript * script, void * arg)
{
	debugger_script_remove(script);
}

//////////////////////////////////////////////////////////////////////////
// debugger_script_getActiveScript
struct activeScript * debugger_script_getActiveScript(JSScript * script)
{
	list<struct activeScript *>::iterator it;

	for(it=debuggerActiveScripts.begin();it!=debuggerActiveScripts.end();it++)
	{
		if((*it)->script==script)
			return (*it);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// debugger_breakpoint_create
struct breakpoint * debugger_breakpoint_create(char * filename, unsigned int line)
{
	struct breakpoint * bp = (struct breakpoint *)malloc(sizeof(struct breakpoint));
	bp->fullPath = strdup(filename);
	bp->line = line;

	struct stat fileStat;
	stat(filename, &fileStat);
	bp->inodeNum = fileStat.st_ino;

	debuggerBreakpoints.push_front(bp);

	debugger_breakpoint_set(bp);

	return bp;
}


//////////////////////////////////////////////////////////////////////////
// debugger_breakpoint_set
void debugger_breakpoint_set(struct breakpoint * bp)
{
	struct activeScript * bestMatch=0;
	list<struct activeScript *>::iterator it;

	for(it=debuggerActiveScripts.begin();it!=debuggerActiveScripts.end();it++)
	{
		if(bp->inodeNum==(*it)->inodeNum && (*it)->startLine<=bp->line && (*it)->endLine>=bp->line)
		{
			if(bestMatch==0 || ((*it)->startLine>bestMatch->startLine || (*it)->endLine<bestMatch->endLine))
				bestMatch = (*it);
		}
	}

	if(bestMatch!=0)
	{
		jsbytecode * pc = JS_LineNumberToPC(bestMatch->cx, bestMatch->script, bp->line);
		JS_SetTrap(bestMatch->cx, bestMatch->script, pc, debugger_breakpoint_handler, 0);
	}

	printf("Breakpoint set at %s:%d\n", bp->fullPath, bp->line);
}


//////////////////////////////////////////////////////////////////////////
// debugger_breakpoint_handler
JSTrapStatus debugger_breakpoint_handler(JSContext * cx, JSScript * script, jsbytecode * pc, jsval * rval, void * arg)
{
	struct activeScript * as = debugger_script_getActiveScript(script);

	unsigned int line = JS_PCToLineNumber(cx, script, pc);
	printf("breakpointed at line: %s:%d\n", as->fullPath, line);

	struct activeContext * ac = debugger_context_getActiveContext(cx);
	ac->suspended = true;

	char buf[8192];
	sprintf(buf, "%d\n%s\n%d\n", ac->num, as->fullPath, line);
	debugger_send_message("breakpoint_reached", buf);

	while(ac->suspended)
	{
		usleep(100);
	}

	return JSTRAP_CONTINUE;
}


//////////////////////////////////////////////////////////////////////////
// debugger_step_handler
JSTrapStatus debugger_step_handler(JSContext * cx, JSScript * script, jsbytecode * pc, jsval * rval, void * arg)
{
	struct debugStep * ds = (struct debugStep *)arg;
	struct activeScript * as = debugger_script_getActiveScript(script);
	unsigned int line = JS_PCToLineNumber(cx, script, pc);
	struct activeContext * ac = debugger_context_getActiveContext(cx);

	if(ds->ac!=ac)
	{
		fprintf(stderr, "contexts don't match in step_handler!\n");
		return JSTRAP_CONTINUE;
	}

	if(ds->type==STEP_INTO || ds->type==STEP_OVER)
	{
		if(ds->as==0)
		{
			ds->as = as;
			ds->lastStopLine = line;
			printf("stepped into or over %d\n", line);
			return JSTRAP_CONTINUE;
		}

		if(ds->as==as && ds->lastStopLine==line)
		{
			fprintf(stderr, "script and line are same, haven't moved\n");
			return JSTRAP_CONTINUE;
		}
	}
	else if(ds->type==STEP_RETURN)
	{
		// TODO: implement
		return JSTRAP_CONTINUE;
	}

		/*  if (db->frameStop != cx->fp &&
   (db->mode == Debugger::Stepout || db->mode == Debugger::Stepover))
      return JSTRAP_CONTINUE;

  if (db->mode == Debugger::Run) return JSTRAP_CONTINUE;

  unsigned int line = JS_PCToLineNumber(cx, script, pc);

  if (
      (
       db->mode == Debugger::Stepping
       || db->mode == Debugger::Stepover
       || db->mode == Debugger::Stepout
      )
      && script == db->lastStop
      && line == db->lastLine
     )
   return JSTRAP_CONTINUE;

  db->lastStop = script;
  db->lastLine = line;*/

	// If we get here, we should notify the debugger that we've reached our end of the step
	free(ds);

	printf("stepped to line: %s:%d\n", as->fullPath, line);
	ac->suspended = true;

	char buf[8192];
	sprintf(buf, "%d\n%s\n%d\n", ac->num, as->fullPath, line);
	debugger_send_message("stepped", buf);

	while(ac->suspended)
	{
		usleep(100);
	}

	return JSTRAP_CONTINUE;
}


//////////////////////////////////////////////////////////////////////////
// debugger_execute_hook
void * debugger_execute_hook(JSContext * cx, JSStackFrame * fp, JSBool before, JSBool * ok, void * arg)
{
	JSScript * script = JS_GetFrameScript(cx, fp);
	jsbytecode * pc = JS_GetFramePC(cx, fp);
	if(pc)
	{
		unsigned int line = JS_PCToLineNumber(cx, script, pc);
		printf("line: %d\n", line);
	}
	int i=0;
	i++;

	return 0;
}


//////////////////////////////////////////////////////////////////////////
// debugger_send_message
void 	debugger_send_message(const char * messageName, const char * messageData)
{
	fprintf(stderr, "Sending Message [%s]\n", messageName);
	fflush(stderr);

	char debuggerURL[4096];
	sprintf(debuggerURL, "http://%s:%d/%s", debuggerRemoteAddress, debuggerRemotePort, messageName);

	CURL * curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, debuggerURL);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, messageData);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
}


//////////////////////////////////////////////////////////////////////////
// debugger_received_message
bool	debugger_received_message(const char * messageName, struct evbuffer * messageData, struct evbuffer * reply)
{
	fprintf(stderr, "Received Message [%s]\n", messageName);
	fflush(stderr);

	if(!strcmp(messageName, "breakpoint_set"))
	{
		char * filename=strdup(evbuffer_readline(messageData));
		unsigned int line=atoi(evbuffer_readline(messageData));
		debugger_breakpoint_create(filename, line);
		free(filename);
		return true;
	}
	else if(!strcmp(messageName, "start"))
	{
		gWaitingForDebugger = false;
		return true;
	}
	else if(!strcmp(messageName, "continue"))
	{
		int threadNum=atoi(evbuffer_readline(messageData));
		struct activeContext * ac = debugger_context_getActiveContextByNum(threadNum);
		ac->suspended = false;

		return true;
	}
	else if(!strcmp(messageName, "step_into") || !strcmp(messageName, "step_over") || !strcmp(messageName, "step_return"))
	{
		int threadNum=atoi(evbuffer_readline(messageData));
		struct activeContext * ac = debugger_context_getActiveContextByNum(threadNum);
		struct debugStep * ds = (struct debugStep *)malloc(sizeof(struct debugStep));
		ds->ac = ac;
		ds->as = 0;
		ds->lastStopLine = 0;

		if(!strcmp(messageName, "step_into"))
		{
			ds->type = STEP_INTO;
		}
		else if(!strcmp(messageName, "step_over"))
		{
			ds->type = STEP_OVER;
		}
		else if(!strcmp(messageName, "step_return"))
		{
			ds->type = STEP_RETURN;
			//debuggerStopFrame = fp;
			// TODO: Something
			// TODO: Something
		}

		ac->suspended = false;

		JS_SetInterrupt(gJSRuntime, debugger_step_handler, ds);

		return true;
	}
	else if(!strcmp(messageName, "stack_get"))
	{
		int threadNum=atoi(evbuffer_readline(messageData));
		struct activeContext * ac = debugger_context_getActiveContextByNum(threadNum);
		JSContext * cx = ac->cx;

		JSStackFrame * fi=0;
		JSStackFrame * fp=0;
		while((fp=JS_FrameIterator(cx, &fi)))
		{
			if(JS_IsNativeFrame(cx, fp))
				break;

			JSScript * script = JS_GetFrameScript(cx, fp);
			jsbytecode * pc = JS_GetFramePC(cx, fp);
			if(!pc)
				break;

			struct activeScript * as = debugger_script_getActiveScript(script);
			evbuffer_add_printf(reply, "%s:%d\n", as->fullPath, JS_PCToLineNumber(cx, script, pc));
		}

		return true;
	}
	else if(!strcmp(messageName, "variables_get"))
	{
		int threadNum=atoi(evbuffer_readline(messageData));
		struct activeContext * ac = debugger_context_getActiveContextByNum(threadNum);
		JSContext * cx = ac->cx;

		JSObject * obj = 0;
		JSStackFrame * fi=0;
		JSStackFrame * fp=0;
		fp=JS_FrameIterator(cx, &fi);
		if(fp)
		{
			obj = JS_GetFrameCallObject(cx, fp);
			if(!obj)
				obj = JS_GetFrameScopeChain(cx, fp);
		}
		if(!obj)
			obj = JS_GetGlobalObject(cx);

		if(obj)
		{
			JSPropertyDescArray pda;
			if(JS_GetPropertyDescArray(cx, obj, &pda))
			{
				for(size_t i=0;i<pda.length;i++)
				{
					if(pda.array[i].flags & (JSPD_ERROR | JSPD_EXCEPTION))
						continue;

					char * value = JS_GetStringBytes(JS_ValueToString(cx, pda.array[i].value));
					char * valueEncoded = base64_encode((unsigned char *)value, strlen(value));
					evbuffer_add_printf(reply, "%s\t%s\t%s\t%s\n", JS_GetStringBytes(JS_ValueToString(cx, pda.array[i].id)), JS_ValueType(cx, &(pda.array[i].value)), "false", valueEncoded);
					free(valueEncoded);
				}
			}
		}

		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// debugger_interface_breakpoint_handler
void 	debugger_webinterface_handler(struct evhttp_request * req, void * arg)
{
	const char * 		requestURI = evhttp_request_uri(req);

	struct evbuffer * 	reply = evbuffer_new();

	if(!debugger_received_message((requestURI+1), req->input_buffer, reply))
		evbuffer_add_printf(reply, "ERROR");

	evhttp_send_reply(req, HTTP_OK, "OK", reply);
	evbuffer_free(reply);
}
