// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

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
#include "command_line.h"
#include "debugger.h"

enum
{
	DEBUG_HOST = 256,
	DEBUG_PORT
};

//////////////////////////////////////////////////////////////////////////
// command_line_handle
char * command_line_handle(int argc, char ** argv)
{
	int		c;
	char * 	programName = argv[0];

	static struct option const long_options[] =
	{
		{ "help",   0, 0, 'h' },
		{ "debugMode",   0, 0, 'd' },
		{ "debugHost", 1, 0, DEBUG_HOST },
		{ "debugPort", 1, 0, DEBUG_PORT },
		{ NULL,     0, NULL, 0 }
	};

	while((c=getopt_long(argc, argv, "hd::", long_options, NULL))!=-1)
	{
		switch(c)
		{
			case 'h':
				command_line_usage(programName, EXIT_SUCCESS);
				break;
			case 'd':
				gDebuggerEnabled = true;
				gWaitingForDebugger = true;
				break;
			case DEBUG_HOST:
				gDebuggerLocalAddress = strdup(optarg);
				break;
			case DEBUG_PORT:
				gDebuggerLocalPort = atoi(optarg);
				break;
			default:
				command_line_usage(programName, EXIT_FAILURE);
		}
	}

	if(optind>=argc)
		command_line_usage(programName, EXIT_FAILURE);

	return argv[optind++];
}


//////////////////////////////////////////////////////////////////////////
// command_line_usage
void command_line_usage(char * program_name, int status)
{
      printf("Usage: %s [OPTION]... FILE\n", program_name);
      printf("Runs the FILE as piston JavaScript.\n\n");
      printf("Options:\n");
      printf("      -h (--help)             display this help and exit\n");
      printf("      -d (--debugMode)        enables debug mode. allows eclipse javascript debugging\n");
      printf("         (--debugHost <host>) specify which host to listen on for incoming debugger connections. default: localhost\n");
      printf("         (--debugPort <port>) specify which port to listen on for incoming debugger connections. default: 7570\n");

      exit(status);
}
