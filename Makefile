CC=g++
CFLAGS=-w -I.
OPCVFLAGS=`pkg-config --cflags --libs opencv`

mapreader: main.cpp tmatch.cpp
	$(CC) main.cpp tmatch.cpp -o mapreader $(CFLAGS) $(OPCVFLAGS)
