ROOTDIR = libhardware
LIBNAME = hardware

CC	= gcc
CFLAGS	= -Wall -ansi -pedantic -m32
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware

###------------------------------
### Main targets 
###------------------------------------------------------------
BINARIES= oracle mmu_manager

all: $(BINARIES)

###------------------------------
### Main rules 
###------------------------------------------------------------

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCDIR)

%: %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

Makefile.depends:
	$(CC) -MM *.c $(INCDIR) > Makefile.depends

include Makefile.depends

oracle: oracle.o matrix.o
mmu_manager: mmu_manager.o user_process.o matrix.o swap.o

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES) Makefile.depends
realclean: clean 
	$(RM) vdiskA.bin vdiskB.bin
