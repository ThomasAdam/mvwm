/* -*-c-*- */

#include <X11/Xutil.h>	/* XClassHint */

#ifndef MVWMLIB_BINDINGS_H_H
#define MVWMLIB_BINDINGS_H_H

/* ---------------------------- global definitions ------------------------- */

#define binding_t_t    unsigned char

/* ---------------------------- global macros ------------------------------ */

#define BIND_IS_KEY_PRESS(t) ((t) == BIND_KEYPRESS || (t) == BIND_PKEYPRESS)
#define BIND_IS_KEY_BINDING(t) ((t) == BIND_KEYPRESS || (t) == BIND_PKEYPRESS)
#define BIND_IS_PKEY_BINDING(t) ((t) == BIND_PKEYPRESS)
#define BIND_IS_MOUSE_BINDING(t) \
	((t) == BIND_BUTTONPRESS || (t) == BIND_BUTTONRELEASE)

/* ---------------------------- type definitions --------------------------- */

typedef enum
{
	/*
	 * press = even number, release = press + 1
	 */
	BIND_BUTTONPRESS = 0,
	BIND_BUTTONRELEASE = 1,
	BIND_KEYPRESS = 2,
	BIND_PKEYPRESS = 4,
} binding_t;

typedef struct Binding
{
	binding_t_t     type;		/* Is it a mouse, key binding? */
	int             Button_Key;	/* Mouse Button number or Keycode */
	char           *key_name;	/* In case of keycode, give the
					 * key_name too
					 */
	int             Context;	/* Mvwm context, ie titlebar, frame,
					 * etc
					 */
	int             Modifier;	/* Modifiers for keyboard state */
	void           *Action;		/* What to do? */
	void           *Action2;	/* This one can be used too */
	char           *windowName;	/* Name of window (regex pattern) this
					 * bindin applies to. NULL means all
					 * windows.
					 */
	struct Binding *NextBinding;
} Binding;

/* ---------------------------- interface functions ------------------------ */

void	 CollectBindingList(Display *, Binding **, Binding **, Bool *,
    binding_t, int, KeySym, int, int, char *);

int	 AddBinding(Display *, Binding **, binding_t, int, KeySym, char *, int,
    int, void *, void *, char *);

void	 FreeBindingStruct(Binding *);
void	 FreeBindingList(Binding *);
void	 RemoveBinding(Binding **, Binding *, Binding *);

Bool	 RemoveMatchingBinding(Display *, Binding **, binding_t, int, KeySym,
    int, int);

void	*CheckBinding(Binding *, int, unsigned int, unsigned int, int,
    binding_t type, const XClassHint * win_class, const char *win_name);

void	*CheckTwoBindings(Bool *, Binding *, int, unsigned int, unsigned int,
    int, binding_t, const XClassHint *, const char *, int, binding_t,
    const XClassHint *, const char *);

void	 GrabWindowKey(Display *, Window, Binding *, unsigned int, unsigned int,
    Bool);

void	 GrabAllWindowKeys(Display *, Window, Binding *, unsigned int,
    unsigned int, Bool);

void	 GrabWindowButton(Display *, Window, Binding *, unsigned int,
    unsigned int, Cursor, Bool);

void	 GrabAllWindowButtons(Display *, Window, Binding *, unsigned int,
    unsigned int, Cursor, Bool);

void	 GrabAllWindowKeysAndButtons(Display *, Window, Binding *,
    unsigned int, unsigned int, Cursor, Bool);

void	 GrabWindowKeyOrButton(Display *, Window, Binding *, unsigned int,
    unsigned int, Cursor, Bool);

KeySym	 MvwmStringToKeysym(Display *, char *);
Bool	 bindingAppliesToWindow(Binding *, const XClassHint *, const char *);
Bool	 is_pass_through_action(const char *);

#endif /* MVWMLIB_BINDINGS_H_H */
