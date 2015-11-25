all:
	cc -o apachelite main.c init.c util.c handler.c -Wall -g -lnsl -lresolv
