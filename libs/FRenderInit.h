/* -*-c-*- */
/* Copyright (C) 2002  Olivier Chapuis */

/* ---------------------------- included header files ---------------------- */

#ifndef F_RENDER_INIT_H
#define F_RENDER_INIT_H

void            FRenderInit(Display *);
int             FRenderGetAlphaDepth(void);
int             FRenderGetErrorCodeBase(void);
int             FRenderGetMajorOpCode(void);
Bool            FRenderGetErrorText(int, char *);
Bool            FRenderGetExtensionSupported(void);

#endif
