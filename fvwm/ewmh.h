/* -*-c-*- */
/* Copyright (C) 2001  Olivier Chapuis */

#ifndef _EWMH_
#define _EWMH_

/* Extended window manager hints support */

/* ewmh_conf.c */
Bool            EWMH_BugOpts(char *, Bool);
void            CMD_EwmhNumberOfDesktops(F_CMD_ARGS);
void            CMD_EwmhBaseStrut(F_CMD_ARGS);
Bool            EWMH_CMD_Style(char *, window_style *, int);

/* for maximize and placement ewmh style */
#define EWMH_IGNORE_WORKING_AREA      0
#define EWMH_USE_WORKING_AREA         1
#define EWMH_USE_DYNAMIC_WORKING_AREA 2
#define EWMH_WORKING_AREA_MASK        3

/* Extended window manager hints support */
#include "libs/FScreen.h"
#include <X11/Xmd.h>

void            EWMH_SetCurrentDesktop(struct monitor *);
void            EWMH_SetNumberOfDesktops(struct monitor *);
void            EWMH_SetDesktopViewPort(struct monitor *);
void            EWMH_SetDesktopGeometry(struct monitor *);

void            EWMH_SetActiveWindow(Window);
void            EWMH_SetWMDesktop(FvwmWindow *);
void            EWMH_SetWMState(FvwmWindow *, Bool);

int             EWMH_IsKdeSysTrayWindow(Window);
void            EWMH_ManageKdeSysTray(Window, int);
void            EWMH_SetClientList(struct monitor *);
void            EWMH_SetClientListStacking(struct monitor *);
void            EWMH_UpdateWorkArea(void);
void            EWMH_GetWorkAreaIntersection(FvwmWindow *fw, int *, int *,
    int *, int *, int);
float           EWMH_GetBaseStrutIntersection(struct monitor *, int,
    int, int, int, Bool);
float           EWMH_GetStrutIntersection(struct monitor *, int, int,
    int, int, Bool);
void            EWMH_SetFrameStrut(FvwmWindow *);
void            EWMH_SetAllowedActions(FvwmWindow *);

void            EWMH_GetIconGeometry(FvwmWindow *, rectangle *);

void            EWMH_GetStyle(FvwmWindow *, window_style *);
void            EWMH_WindowInit(FvwmWindow *);
void            EWMH_RestoreInitialStates(FvwmWindow *, int);
void            EWMH_DestroyWindow(FvwmWindow *);
void            EWMH_WindowDestroyed(void);

void            EWMH_Init(struct monitor *);
void            EWMH_ExitStuff(void);

/* ewmh_conf.c */

/* ewmh_events.c */
Bool            EWMH_ProcessClientMessage(const exec_context_t *);
void            EWMH_ProcessPropertyNotify(const exec_context_t *);

/* ewmh_icon.c */
void            EWMH_DeleteWmIcon(FvwmWindow *, Bool, Bool);
int             EWMH_SetIconFromWMIcon(FvwmWindow *, CARD32 *, int, Bool);
void            EWMH_DoUpdateWmIcon(FvwmWindow *, Bool, Bool);

/* ewmh_name.c */
void            EWMH_SetVisibleName(FvwmWindow *, Bool);
int             EWMH_WMName(FvwmWindow *, XEvent *, window_style *,
    unsigned long);
int             EWMH_WMIconName(FvwmWindow *, XEvent *,
    window_style *, unsigned long);
void            EWMH_SetDesktopNames(struct monitor *);
void            EWMH_fullscreen(FvwmWindow *);

#endif /* _EWMH_ */
