lab1: main.c types.c matrix.c tests.c
	gcc -o lab1 main.c types.c matrix.c tests.c -lm

clean:
	rm -f lab1
