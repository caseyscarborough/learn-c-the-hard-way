CFLAGS=-Wall -g

all:
	cc ex1.c -o ex1
	cc ex3.c -o ex3
	cc ex5.c -o ex5

clean:
	rm ex1 ex3 ex5