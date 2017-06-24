CC=g++
CFLAGS=-I.

write_script: write_script.cpp
	$(CC) -o write_script write_script.cpp $(CFLAGS)

#g++ template_match.cpp -o template `pkg-config --cflags --libs opencv`
