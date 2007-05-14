bin_PROGRAMS = gen
pkglib_LTLIBRARIES=libGL.la

gen_SOURCES = gen.c
gen_LDFLAGS = `pkg-config --libs libxml-2.0`
CFLAGS = @CFLAGS@ `pkg-config --cflags libxml-2.0` -I/usr/include/ 

wrapped.c: gen 
	@./gen

dewrapped.c: gen
	@./gen

libGL_la_SOURCES = dewrapped.c wrapped.c array.c client.c fifo.c get.c id.c init.c lib_funcs.c map.c matrix.c multi_screen.c overrides.c pbuffer.c segment.c server.c single_screen.c tile_screen.c window.c

wrapped.o: wrapped.c

dewrapped.o: dewrapped.c

CLEANFILES = wrapped.c wrapped.h dewrapped.c dewrapped.h


