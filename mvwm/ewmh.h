/* -*-c-*- */
/* Copyright (C) 2001  Olivier Chapuis */

#ifndef _EWMH_
#define _EWMH_

/* Extended window manager hints support */

/* ewmh_conf.c */
Bool EWMH_BugOpts(char *opt, Bool toggle);
void CMD_EwmhNumberOfDesktops(F_CMD_ARGS);
void CMD_EwmhBaseStrut(F_CMD_ARGS);
Bool EWMH_CMD_Style(char *token, window_style *ptmpstyle, int on);

/* for maximize and placement ewmh style */
#define EWMH_IGNORE_WORKING_AREA      0
#define EWMH_USE_WORKING_AREA         1
#define EWMH_USE_DYNAMIC_WORKING_AREA 2
#define EWMH_WORKING_AREA_MASK        3

/* Extended window manager hints support */
#include "libs/FScreen.h"
#include <X11/Xmd.h>


void EWMH_SetCurrentDesktop(struct monitor *);
void EWMH_SetNumberOfDesktops(struct monitor *);
void EWMH_SetDesktopViewPort(struct monitor *);
void EWMH_SetDesktopGeometry(struct monitor *);

void EWMH_SetActiveWindow(Window w);
void EWMH_SetWMDesktop(MvwmWindow *fw);
void EWMH_SetWMState(MvwmWindow *fw, Bool do_restore);

int EWMH_IsKdeSysTrayWindow(Window w);
void EWMH_ManageKdeSysTray(Window w, int type);
void EWMH_SetClientList(struct monitor *);
void EWMH_SetClientListStacking(struct monitor *);
void EWMH_UpdateWorkArea(void);
void EWMH_GetWorkAreaIntersection(
	MvwmWindow *fw, int *x, int *y, int *w, int *h, int type);
float EWMH_GetBaseStrutIntersection(struct monitor *,
	int x11, int y11, int x12, int y12, Bool use_percent);
float EWMH_GetStrutIntersection(struct monitor *,
	int x11, int y11, int x12, int y12, Bool use_percent);
void EWMH_SetFrameStrut(MvwmWindow *fw);
void EWMH_SetAllowedActions(MvwmWindow *fw);

void EWMH_GetIconGeometry(MvwmWindow *fw, rectangle *icon_rect);

void EWMH_GetStyle(MvwmWindow *fw, window_style *style);
void EWMH_WindowInit(MvwmWindow *fw);
void EWMH_RestoreInitialStates(MvwmWindow *fw, int event_type);
void EWMH_DestroyWindow(MvwmWindow *fw);
void EWMH_WindowDestroyed(void);

void EWMH_Init(struct monitor *);
void EWMH_ExitStuff(void);

/* ewmh_conf.c */

/* ewmh_events.c */
Bool EWMH_ProcessClientMessage(const exec_context_t *exc);
void EWMH_ProcessPropertyNotify(const exec_context_t *exc);

/* ewmh_icon.c */
void EWMH_DeleteWmIcon(MvwmWindow *fw, Bool mini_icon, Bool icon);
int EWMH_SetIconFromWMIcon(
	MvwmWindow *fw, CARD32 *list, int size, Bool is_mini_icon);
void EWMH_DoUpdateWmIcon(MvwmWindow *fw, Bool mini_icon, Bool icon);

/* ewmh_name.c */
void EWMH_SetVisibleName(MvwmWindow *fw, Bool is_icon_name);
int EWMH_WMName(
	MvwmWindow *fw, XEvent *ev, window_style *style, unsigned long any);
int EWMH_WMIconName(
	MvwmWindow *fw, XEvent *ev, window_style *style, unsigned long any);
void EWMH_SetDesktopNames(struct monitor *);
void EWMH_fullscreen(MvwmWindow *fw);

#endif /* _EWMH_ */
