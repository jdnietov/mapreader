CC=gcc
CFLAGS=-I.

write_script: write_script.c
	$(CC) -o write_script write_script.c $(CFLAGS)
