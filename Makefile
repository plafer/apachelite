all:
	cc -o apachelite main.c init.c util.c handler.c gbuf.c parser.c structs.c -Wall -g -lnsl -lresolv
