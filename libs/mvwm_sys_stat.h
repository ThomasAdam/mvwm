#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifndef S_IRWXO
#	define MVWM_S_IRWXO 0007
#else
#	define MVWM_S_IRWXO S_IRWXO
#endif
#ifndef S_ISUID
#	define MVWM_S_ISUID 0004000
#else
#	define MVWM_S_ISUID S_ISUID
#endif
#ifndef S_ISGID
#	define MVWM_S_ISGID 0002000
#else
#	define MVWM_S_ISGID S_ISGID
#endif
#ifndef S_ISVTX
#	define MVWM_S_ISVTX 0001000
#else
#	define MVWM_S_ISVTX S_ISVTX
#endif
#ifndef S_IRWXU
#	define MVWM_S_IRWXU 00700
#else
#	define MVWM_S_IRWXU S_IRWXU
#endif
#ifndef S_IRUSR
#	define MVWM_S_IRUSR 00400
#else
#	define MVWM_S_IRUSR S_IRUSR
#endif
#ifndef S_IWUSR
#	define MVWM_S_IWUSR 00200
#else
#	define MVWM_S_IWUSR S_IWUSR
#endif
#ifndef S_IXUSR
#	define MVWM_S_IXUSR 00100
#else
#	define MVWM_S_IXUSR S_IXUSR
#endif
#ifndef S_IRWXG
#	define MVWM_S_IRWXG 00070
#else
#	define MVWM_S_IRWXG S_IRWXG
#endif
#ifndef S_IRGRP
#	define MVWM_S_IRGRP 00040
#else
#	define MVWM_S_IRGRP S_IRGRP
#endif
#ifndef S_IWGRP
#	define MVWM_S_IWGRP 00020
#else
#	define MVWM_S_IWGRP S_IWGRP
#endif
#ifndef S_IXGRP
#	define MVWM_S_IXGRP 00010
#else
#	define MVWM_S_IXGRP S_IXGRP
#endif
#ifndef S_IRWXO
#	define MVWM_S_IRWXO 00007
#else
#	define MVWM_S_IRWXO S_IRWXO
#endif
#ifndef S_IROTH
#	define MVWM_S_IROTH 00004
#else
#	define MVWM_S_IROTH S_IROTH
#endif
#ifndef S_IWOTH
#	define MVWM_S_IWOTH 00002
#else
#	define MVWM_S_IWOTH S_IWOTH
#endif
#ifndef S_IXOTH
#	define MVWM_S_IXOTH 00001
#else
#	define MVWM_S_IXOTH S_IXOTH
#endif
#ifndef S_ISLNK
#	define MVWM_S_ISLNK(x) 0
#else
#	define MVWM_S_ISLNK(x) S_ISLNK(x)
#endif
#ifndef S_IFLNK
#	define MVWM_S_IFLNK 00000
#else
#	define MVWM_S_IFLNK S_IFLNK
#endif
