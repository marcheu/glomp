SUBDIRS=generate
pkglib_LTLIBRARIES=libGL.la

libGL_la_SOURCES = dewrapped.c wrapped.c wrapped.h fifo.c init.c overrides.c

wrapped.c: generate/generate
	@./generate/generate

CLEANFILES = wrapped.c wrapped.h dewrapped.c dewrapped.h


