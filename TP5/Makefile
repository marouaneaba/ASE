#ROOTDIR=/home/enseign/ASE

ROOTDIR=libhardware

CC	= gcc
#CFLAGS	= -m32 -Wall -ansi -pedantic 
CFLAGS	= -m32 -ansi 
CFLAGS  += -g
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware

###------------------------------
### Main targets 
###------------------------------
BINARIES= dmps vm
OBJECTS	= $(addsuffix .o,\
	  drive mbr dmps vol inode ifile)

all: $(BINARIES) $(OBJECTS)

test: test_convert test_vol


###------------------------------
### Main rules 
###------------------------------------------------------------
dmps.o: dmps.c
	$(CC) $(CFLAGS) -c dmps.c $(INCDIR)

dmps: dmps.o drive.o
	$(CC) $(CFLAGS) -o dmps dmps.o drive.o $(LIBS)
	
drive.o: drive.c drive.h constantes.h
	$(CC) $(CFLAGS) -c drive.c drive.h $(INCDIR)

inode.o: inode.c inode.h constantes.h vol.o
	$(CC) $(CFLAGS) -c inode.c inode.h $(INCDIR)
	
ifile.o: ifile.c ifile.h constantes.h inode.o
	$(CC) $(CFLAGS) -c ifile.c ifile.h $(INCDIR)
	
mbr.o: mbr.c mbr.h drive.h constantes.h
	$(CC) $(CFLAGS) -c mbr.c mbr.h $(INCDIR)

vm.o: vm.c drive.c drive.h constantes.h mbr.o
	$(CC) $(CFLAGS) -c vm.c $(INCDIR)
			
vm: vm.o
	$(CC) $(CFLAGS) -o vm vm.o drive.o mbr.o $(LIBS)
	
vol.o: vol.c vol.h
	$(CC) $(CFLAGS) -c vol.c vol.h $(INCDIR)


###------------------------------
### Testing rules
###------------------------------------------------------------

#TESTDIR = test/

test_convert: drive.o constantes.h mbr.o vol.o test_convert.c
	$(CC) $(CFLAGS) -o test_convert drive.o mbr.o constantes.h vol.o test_convert.c ${LIBS} ${INCDIR}

test_vol: drive.o constantes.h mbr.o vol.o test_vol.c
	$(CC) $(CFLAGS) -o test_vol drive.o mbr.o constantes.h vol.o test_vol.c ${LIBS} ${INCDIR}



###------------------------------
### Misc.
###------------------------------
.PHONY: clean realclean
clean:
	$(RM) *.o $(BINARIES)
realclean: clean 
	$(RM) vdiskA.bin vdiskB.bin test_convert test_vol
