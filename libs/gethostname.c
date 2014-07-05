/* -*-c-*- */
/* gethostname emulation for SysV and POSIX.1.
   Copyright (C) 1992 Free Software Foundation, Inc. */
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* David MacKenzie <djm@gnu.ai.mit.edu> */

#include "config.h"

#ifdef HAVE_UNAME
#include <sys/utsname.h>
#endif

/* Put up to LEN chars of the host name into NAME.
   Null terminate it if the name is shorter than LEN.
   Return 0 if ok, -1 if error.  */

int
gethostname(char *name, int len)
{
#ifdef HAVE_UNAME
	struct utsname  uts;

	if (uname(&uts) == -1)
		return -1;
	if (len > sizeof(uts.nodename)) {
		/*
		 * More space than we need is available.
		 */
		name[sizeof(uts.nodename)] = '\0';
		len = sizeof(uts.nodename);
	}
	strncpy(name, uts.nodename, len);
#else
	/*
	 * Hardcode your system name if you want.
	 */
	strcpy(name, "");
#endif
	return 0;
}
