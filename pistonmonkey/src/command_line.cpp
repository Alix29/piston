// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

// local includes
#include "pistonmonkey.h"
#include "command_line.h"


//////////////////////////////////////////////////////////////////////////
// command_line_handle
char * command_line_handle(int argc, char ** argv)
{
	int		c;
	char * 	programName = argv[0];

	static struct option const long_options[] =
	{
		{ "help",   0, 0, 'h' },
		{ "jsed",   0, 0, 'j' },
		{ "waitfordebug",   0, 0, 'w' },
		{ NULL,     0, NULL, 0 }
	};

	while((c=getopt_long(argc, argv, "hjw", long_options, NULL))!=-1)
	{
		switch(c)
		{
			case 'h':
				command_line_usage(programName, EXIT_SUCCESS);
				break;
			case 'j':
				gDebuggerEnabled = true;
				break;
			case 'w':
				gWaitingForDebugger = true;
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
      printf("Runs the FILE as pistonmonkey JavaScript.\n\n");
      printf("Options:\n");
      printf("      -h (--help)             display this help and exit\n");
      printf("      -j (--jsed)             run in 'jsed' mode. allows eclipse javascript debugging\n");
      printf("      -w (--waitfordebug)     wait for the jsed debugger to attach before executing script\n");

      exit(status);
}
