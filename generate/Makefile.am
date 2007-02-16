YACC=@YACC@ -d
bin_PROGRAMS = generate
y.tab.c: gen.y
	yacc -d gen.y 
lex.yy.c: gen.l
	lex gen.l 
generate_SOURCES = y.tab.c lex.yy.c
CLEANFILES = y.tab.c lex.yy.c

