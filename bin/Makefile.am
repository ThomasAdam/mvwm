## Process this file with automake to create Makefile.in

bin_PROGRAMS = mvwm-root

mvwm_root_SOURCE= mvwm-root.c
mvwm_root_DEPENDENCIES = $(top_builddir)/libs/libmvwm.a

LDADD = -L$(top_builddir)/libs $(X_LIBS) -lmvwm $(Xcursor_LIBS) \
	$(X_PRE_LIBS) -lXext -lX11 -lm $(X_EXTRA_LIBS) $(Xrender_LIBS) \
	$(Xcursor_LIBS) $(png_LIBS)
AM_CPPFLAGS = -I$(top_srcdir) $(X_CFLAGS) $(png_CFLAGS)
