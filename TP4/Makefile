ROOTDIR=/home/enseign/ASE

CC	= gcc
CFLAGS	= -m32 -Wall -pedantic
CFLAGS  += -g
INCS    = -I$(ROOTDIR)/include
LIBS    = -L$(ROOTDIR)/lib -lhardware

###------------------------------
### Main rules 
###------------------------------------------------------------	
main: context.o prodcons.o sem.o hw_config.h
	$(CC) $(CFLAGS) -o main context.o prodcons.o sem.o $(LIBS)
prodcons.o: prodcons.c prodcons.h context.h sem.h
	$(CC) $(CFLAGS) $(INCS) -c prodcons.c prodcons.h
sem.o: sem.c sem.h context.h
	$(CC) $(CFLAGS) $(INCS) -c sem.c sem.h
context.o: context.c context.h hw_config.h
	$(CC) $(CFLAGS) $(INCS) -c context.c context.h


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o *.gch *.bin
realclean: clean 
	$(RM) tmr
