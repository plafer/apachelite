all:
	cc -o apachelite main.c init.c util.c -Wall -g -lnsl -lresolv
