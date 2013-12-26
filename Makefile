CFLAGS=-Wall -g

all:
	mkdir -p bin
	cc ex1.c -o ./bin/ex1
	cc ex3.c -o ./bin/ex3
	cc ex5.c -o ./bin/ex5
	cc ex6.c -o ./bin/ex6
	cc ex7.c -o ./bin/ex7

clean:
	rm bin/*