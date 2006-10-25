SUBDIRS=generate
pkglib_LTLIBRARIES=libGL.la

libGL_la_SOURCES = wrapped.c wrapped.h fifo.c overrides.c

wrapped.c: generate/generate
	@./generate/generate


