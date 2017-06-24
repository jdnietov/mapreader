CC=g++
CFLAGS=-I.

write_script: write_script.cpp
	$(CC) -o write_script write_script.cpp $(CFLAGS)
