all: prog

prog: displaystack.o try.o try_mul.o
	gcc -m32 -o prog displaystack.o try.o try_mul.o

displaystack.o: displaystack.c try.o
	gcc -m32 -o displaystack.o -c displaystack.c

try_mul.o: try_mul.c try.o
	gcc -m32 -o try_mul.o -c try_mul.c

try.o: try.c try.h
	gcc -m32 -o try.o -c try.c

clean:
	rm -rf *.o

mrproper: clean
	rm -rf prog
