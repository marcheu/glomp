bin_PROGRAMS = gen
pkglib_LTLIBRARIES=libGL.la

gen_SOURCES = gen.c
gen_LDFLAGS = `pkg-config --libs libxml-2.0`
CFLAGS = `pkg-config --cflags libxml-2.0`

wrapped.c: gen 
	@./gen

dewrapped.c: gen
	@./gen

libGL_la_SOURCES = dewrapped.c wrapped.c fifo.c init.c overrides.c transfertFenetre.c

wrapped.o: wrapped.c

dewrapped.o: dewrapped.c

CLEANFILES = wrapped.c wrapped.h dewrapped.c dewrapped.h


