all:
	gcc main.c cipher.c hash.c -O3 -o test

debug:
	gcc -g main.c cipher.c hash.c -O3 -o test
