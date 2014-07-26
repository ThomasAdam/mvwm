/* -*-c-*- */

/* Wrappers for io functions from the standard library that might be
 * interrupted.  These functions are restarted when necessary.
 */

/* ---------------------------- interface functions ------------------------ */

ssize_t mvwm_send(int s, const void *buf, size_t len, int flags);
ssize_t mvwm_recv(int s, void *buf, size_t len, int flags);
