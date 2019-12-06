CPPFLAGS=-std=c++11 -g -O3 -Wall
SERVER=server
CLIENT=client

all:
	g++ $(CPPFLAGS) proj11.server.c -o server
	g++ $(CPPFLAGS) proj11.client.c -o client

clean: 
	rm -f *.o server client