# -*- Makefile -*-
#
#

AM_CPPFLAGS = \
	$(MPI_CPPFLAGS)

compdir = $(pkglibdir)
comp_LTLIBRARIES = libfooprojbar.la
libfooprojbar_la_SOURCES = \
	Dummy.h \
	Dummy.cc

EXTRA_DIST = \
    dummy.py

libfooprojbar_la_LDFLAGS = -module -avoid-version

install-exec-hook:
	$(SST_REGISTER_TOOL) SST_FOOPROJBAR_SOURCE fooprojbar=$(abs_srcdir)
	$(SST_REGISTER_TOOL) SST_FOOPROJBAR_TESTS  fooprojbar=$(abs_srcdir)

##########################################################################
##########################################################################
##########################################################################


