generateur: y.tab.c lex.yy.c
	gcc -o generateur  y.tab.c  lex.yy.c -ll
y.tab.c: gen.y
	yacc -d gen.y 
lex.yy.c: gen.l
	lex gen.l 

clean:
	rm -f y.tab.* lex.yy.c y.output wrapped.c wrapped.h dewrapped.c dewrapped.h
	rm -f *~
	rm -f analyseur
