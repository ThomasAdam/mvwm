/* -*-c-*- */

#ifndef _STACK_H
#define _STACK_H

#define DEBUG_STACK_RING 1
#ifdef DEBUG_STACK_RING
void            verify_stack_ring_consistency(void);
#endif
void            remove_window_from_stack_ring(MvwmWindow *t);
void            add_window_to_stack_ring_after(MvwmWindow *t,
    MvwmWindow *add_after_win);
MvwmWindow     *get_next_window_in_stack_ring(const MvwmWindow *t);
MvwmWindow     *get_prev_window_in_stack_ring(const MvwmWindow *t);
MvwmWindow     *get_transientfor_mvwmwindow(const MvwmWindow *t);
Bool            position_new_window_in_stack_ring(MvwmWindow *t,
    Bool do_lower);
void            RaiseWindow(MvwmWindow *t, Bool is_client_request);
void            LowerWindow(MvwmWindow *t, Bool is_client_request);
Bool            HandleUnusualStackmodes(unsigned int stack_mode,
    MvwmWindow *r, Window rw, MvwmWindow *sib, Window sibw);
void            BroadcastRestackAllWindows(void);
void            BroadcastRestackThisWindow(MvwmWindow *t);

int             compare_window_layers(MvwmWindow *t, MvwmWindow *s);
void            set_default_layer(MvwmWindow *t, int layer);
void            set_layer(MvwmWindow *t, int layer);
int             get_layer(MvwmWindow *t);
void            new_layer(MvwmWindow *t, int layer);

void            init_stack_and_layers(void);
Bool            is_on_top_of_layer(MvwmWindow *t);
Bool            is_on_top_of_layer_and_above_unmanaged(MvwmWindow *t);

/* This function recursively finds the transients of the window t and sets
 * their is_in_transient_subtree flag.  If a layer is given, only windows in
 * this layer are checked.  If the layer is < 0, all windows are considered. */
#define MARK_RAISE   0
#define MARK_LOWER   1
#define MARK_RESTACK 2
#define MARK_ALL     3
#define MARK_ALL_LAYERS -1
void            mark_transient_subtree(MvwmWindow *t, int layer,
    int mark_mode, Bool do_ignore_icons, Bool use_window_group_hint);

#endif /* _STACK_H */
