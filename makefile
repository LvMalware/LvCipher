all:
	gcc main.c cipher.c hash.c -o test

debug:
	gcc -g main.c cipher.c hash.c -o test
