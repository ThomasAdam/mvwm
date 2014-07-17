/* -*-c-*- */

#ifndef _STACK_H
#define _STACK_H

#define DEBUG_STACK_RING 0
#ifdef DEBUG_STACK_RING
void            verify_stack_ring_consistency(void);
#endif
void            remove_window_from_stack_ring(FvwmWindow *);
void            add_window_to_stack_ring_after(FvwmWindow *, FvwmWindow *);
FvwmWindow     *get_next_window_in_stack_ring(const FvwmWindow *);
FvwmWindow     *get_prev_window_in_stack_ring(const FvwmWindow *);
FvwmWindow     *get_transientfor_fvwmwindow(const FvwmWindow *);
Bool            position_new_window_in_stack_ring(FvwmWindow *, Bool);
void            RaiseWindow(FvwmWindow *, Bool);
void            LowerWindow(FvwmWindow *, Bool);
Bool            HandleUnusualStackmodes(unsigned int, FvwmWindow *, Window,
    FvwmWindow *, Window);
void            BroadcastRestackAllWindows(void);
void            BroadcastRestackThisWindow(FvwmWindow *);

int             compare_window_layers(FvwmWindow *, FvwmWindow *);
void            set_default_layer(FvwmWindow *, int);
void            set_layer(FvwmWindow *, int);
int             get_layer(FvwmWindow *);
void            new_layer(FvwmWindow *, int);

void            init_stack_and_layers(void);
Bool            is_on_top_of_layer(FvwmWindow *);
Bool            is_on_top_of_layer_and_above_unmanaged(FvwmWindow *);

/* This function recursively finds the transients of the window t and sets
 * their is_in_transient_subtree flag.  If a layer is given, only windows in
 * this layer are checked.  If the layer is < 0, all windows are considered. */
#define MARK_RAISE   0
#define MARK_LOWER   1
#define MARK_RESTACK 2
#define MARK_ALL     3
#define MARK_ALL_LAYERS -1
void            mark_transient_subtree(FvwmWindow *, int, int, Bool, Bool);

#endif /* _STACK_H */
