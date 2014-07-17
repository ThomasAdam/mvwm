/* -*-c-*- */

#ifndef _MENUS_
#define _MENUS_

#define MENU_IS_LEFT  0x01
#define MENU_IS_RIGHT 0x02
#define MENU_IS_UP    0x04
#define MENU_IS_DOWN  0x08

/*
 * MISCELLANEOUS MENU STUFF
 */

#define IS_MENU_RETURN(x) \
  ((x)==MENU_DONE || (x)==MENU_ABORTED || (x)==MENU_SUBMENU_TORN_OFF)

struct MenuRoot;
struct MenuStyle;
struct MenuReturn;
struct MenuParameters;
struct MenuOptions;
struct MenuItem;
struct MenuRepaintTransparentParameters;

/*
 * EXPORTED FUNCTIONS
 */

void            menus_init(void);
struct MenuRoot *menus_find_menu(char *);
void            menus_remove_style_from_menus(struct MenuStyle *);
struct MenuRoot *FollowMenuContinuations(struct MenuRoot *,
    struct MenuRoot **);
struct MenuRoot *NewMenuRoot(char *);
void            AddToMenu(struct MenuRoot *, char *, char *, Bool, Bool,
    Bool);
void            menu_enter_tear_off_menu(const exec_context_t *);
void            menu_close_tear_off_menu(FvwmWindow *);
Bool            menu_redraw_transparent_tear_off_menu(FvwmWindow *, Bool);
void            do_menu(struct MenuParameters *, struct MenuReturn *);
char           *get_menu_options(char *, Window, FvwmWindow *,
    XEvent *, struct MenuRoot *, struct MenuItem *,
    struct MenuOptions *);
Bool            DestroyMenu(struct MenuRoot *, Bool, Bool);
void            add_another_menu_item(char *);
void            change_mr_menu_style(struct MenuRoot *, char *);
void            UpdateAllMenuStyles(void);
void            UpdateMenuColorset(int);
void            SetMenuCursor(Cursor);
void            update_transparent_menu_bg(struct
    MenuRepaintTransparentParameters *, int, int, int, int, int, int);
void            repaint_transparent_menu(struct
    MenuRepaintTransparentParameters *, Bool, int, int, int, int, Bool);
Bool            menu_expose(XEvent *event, FvwmWindow *);
int             menu_binding(Display *, binding_t, int, KeySym, int, int,
    char *, char *);
#endif /* _MENUS_ */
