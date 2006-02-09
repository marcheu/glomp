APPLI1 = generate
CSRC1 = $(shell echo *.c)
CC = gcc
LIB = -lm 
OPT = -O0 -g -I/usr/X11R6/include -Wall 

all:	$(APPLI1)
	@echo Ok.

.c.o:
	$(CC) $(OPT) -c $*.c -Wall

COBJ1 = $(CSRC1:.c=.o)

$(APPLI1):$(COBJ1)
	$(CC) $(OPT) -o $(APPLI1) $(COBJ1) $(LIB)

depend:
	makedepend $(CSRC1)
	
clean:
	@echo Cleaning up...
	@rm -f _tout.c nohup.out core gmon.out *.o *~ $(APPLI1)
	@echo Done.


