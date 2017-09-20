ROOTDIR=/home/enseign/ASE

CC	= gcc
CFLAGS	= -m32 -Wall -pedantic  
CFLAGS  += -g
INCS    = -I$(ROOTDIR)/include
LIBS    = -L$(ROOTDIR)/lib -lhardware

###------------------------------
### Main rules 
###------------------------------------------------------------
main: context.o tp3.o hw_config.h
	$(CC) $(CFLAGS) -o main context.o tp3.o $(LIBS)
context.o: context.c context.h hw_config.h
	$(CC) $(CFLAGS) $(INCS) -c context.c context.h
tp3.o: tp3.c tp3.h context.h
	$(CC) $(CFLAGS) $(INCS) -c tp3.c tp3.h


###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o 
realclean: clean 
	$(RM) tmr
