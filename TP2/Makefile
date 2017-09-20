CC=cc -g

main: context.o tp2.o
	$(CC) -m32 -o main context.o tp2.o
context.o: context.c context.h
	$(CC) -m32 -c context.c context.h
tp2.o: tp2.c tp2.h context.h
	$(CC) -m32 -c tp2.c tp2.h

clean:
	rm -rf *.o *.gch *~

all: clean main
