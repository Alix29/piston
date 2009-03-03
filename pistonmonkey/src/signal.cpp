// platform includes
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// local includes
#include "pistonmonkey.h"
#include "signal.h"


//////////////////////////////////////////////////////////////////////////
// signal_init
void signal_init()
{
	if(signal(SIGPIPE, SIG_IGN)==SIG_ERR)
		exit(1);
	if(signal(SIGINT, signal_exit)==SIG_ERR)
		exit(1);
	if(signal(SIGTERM, signal_exit)==SIG_ERR)
		exit(1);
}


//////////////////////////////////////////////////////////////////////////
// signal_exit
void signal_exit(int sig)
{
    gRunning = false;
}


