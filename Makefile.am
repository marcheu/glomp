bin_PROGRAMS = gen
pkglib_LTLIBRARIES=libGL.la

gen_SOURCES = gen.c
gen_LDFLAGS = `pkg-config --libs libxml-2.0`
CFLAGS = `pkg-config --cflags libxml-2.0`

wrapped.c: gen 
	@./gen

dewrapped.c: gen
	@./gen

libGL_la_SOURCES = client.c dewrapped.c fifo.c id.c init.c overrides.c server.c transfertFenetre.c wrapped.c 

wrapped.o: wrapped.c

dewrapped.o: dewrapped.c

CLEANFILES = wrapped.c wrapped.h dewrapped.c dewrapped.h


