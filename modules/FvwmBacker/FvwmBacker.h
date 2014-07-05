/* -*-c-*- */
/* FvwmBacker Module for Fvwm.
 *
 *  Copyright 1994,  Mike Finger (mfinger@mermaid.micro.umn.edu or
 *                               Mike_Finger@atk.com)
 *
 * The author makes not guarantees or warantees, either express or
 * implied.  Feel free to use any contained here for any purpose, as long
 * and this and any other applicible copyrights are kept intact.

 * The functions in this source file that are based on part of the FvwmIdent
 * module for Fvwm are noted by a small copyright atop that function, all
 * others are copyrighted by Mike Finger.  For those functions modified/used,
 * here is the full, original copyright:
 *
 * Copyright 1994, Robert Nation and Nobutaka Suzuki.
 * No guarantees or warantees or anything
 * are provided or implied in any way whatsoever. Use this program at your
 * own risk. Permission to use this program for any purpose is given,
 * as long as the copyright is kept intact. */

#include "libs/fvwmlib.h"

/* Function Prototypes */

void            EndLessLoop(void);
void            ReadFvwmPipe(void);
void            ProcessMessage(unsigned long type, unsigned long *body);
RETSIGTYPE      DeadPipe(int nonsense);
void            ParseConfig(void);
int             ParseConfigLine(char *line);
void            AddCommand(char *line);
