#ifndef LIB_EVENT_H
#define LIB_EVENT_H

/*
 * Return the subwindow member of an event if the event type has one.
 */
Window	 GetSubwindowFromEvent(Display *, const XEvent *);

#endif /* LIB_EVENT_H */
