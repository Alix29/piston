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
		{ "enableDebugger",   0, 0, 'd' },
		{ NULL,     0, NULL, 0 }
	};

	while((c=getopt_long(argc, argv, "hd", long_options, NULL))!=-1)
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
      printf("      -j (--enableDebugger)   enables the debugger. allows eclipse javascript debugging\n");

      exit(status);
}
