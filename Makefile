lab1: main.c types.c matrix.c 
	gcc -o lab1 main.c types.c matrix.c 

clean:
	rm -f lab1 tests

tests: tests.c test_algebra.c matrix.c types.c
	gcc -o tests tests.c test_algebra.c matrix.c types.c
