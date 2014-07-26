/* -*-c-*- */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include "libs/ftime.h"
#include <sys/wait.h>

#if HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <errno.h>

#include "libs/Module.h"
#include "libs/mvwmlib.h"
#include "mvwm/mvwm.h"
#include "libs/vpacket.h"
#include "libs/mvwm_sys_stat.h"

#ifndef HAVE_STRERROR
extern char    *sys_errlist[];
#define strerror(_e)    (sys_errlist[_e])
#endif

#define F_NAME  "MvwmCommand-"

/* number of default arguments when invoked from mvwm */
#define FARGS 6

#define SOL  sizeof( unsigned long )

#ifndef HAVE_MKFIFO
#define mkfifo(path, mode) ((errno = ENOSYS) - ENOSYS - 1)
#endif

char           *fifos_get_default_name(void);
