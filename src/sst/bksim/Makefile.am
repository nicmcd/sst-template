# -*- Makefile -*-
#
#

AM_CPPFLAGS = \
	$(MPI_CPPFLAGS)

compdir = $(pkglibdir)
comp_LTLIBRARIES = libbksim.la
libbksim_la_SOURCES = \
	Worker.h \
	Worker.cc

EXTRA_DIST = \
    README \
    bksim.py

libbksim_la_LDFLAGS = -module -avoid-version

install-exec-hook:
	$(SST_REGISTER_TOOL) SST_BKSIM_SOURCE bksim=$(abs_srcdir)
	$(SST_REGISTER_TOOL) SST_BKSIM_TESTS  bksim=$(abs_srcdir)

##########################################################################
##########################################################################
##########################################################################


