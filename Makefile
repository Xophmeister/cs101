all:
	gcc test.c linkedList.c -o test

clean:
	rm -rf *o test
