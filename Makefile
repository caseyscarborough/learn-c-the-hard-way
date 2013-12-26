CFLAGS=-Wall -g

all:
	cc ex1.c -o ex1
	cc ex3.c -o ex3
	cc ex5.c -o ex5
	cc ex6.c -o ex6
	cc ex7.c -o ex7

clean:
	rm ex1 ex3 ex5 ex6 ex7