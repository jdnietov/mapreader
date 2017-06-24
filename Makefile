CC=g++
CFLAGS=-I.

write_script: write_script.c
	$(CC) -o write_script write_script.cpp $(CFLAGS)
