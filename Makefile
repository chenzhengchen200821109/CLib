


mem_test: mem.c error.c
	gcc -g -Wall -m32 -o mem_test mem_test.c mem.c error.c

clean:
	rm -rf mem_test a.out test
