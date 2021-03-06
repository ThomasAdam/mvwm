/* -*-c-*- */
#include <sys/types.h>

#if HAVE_SYS_BSDTYPES_H
#include <sys/bsdtypes.h>
#endif

#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <errno.h>

#include "libs/mvwmlib.h"
#include "libs/Module.h"
#include "libs/mvwmsignal.h"

#define S_NAME  "/.MvwmConsole-Socket"
/* Prompts for readline support */
#define PS1     ""
#define PS2     ">"

#define HISTSIZE 100    /* readline history file size */
#define HISTFILE "/.MvwmConsole-History"

#define XTERM "xterm"

/* message to client */
#define C_BEG   "_C_Config_Line_Begin_\n"
#define C_END   "_C_Config_Line_End_\n"
#define C_CLOSE "_C_Socket_Close_\n"

#define MAX_COMMAND_SIZE 1000
#define MAX_MESSAGE_SIZE 260
