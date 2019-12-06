CPPFLAGS=-std=c++14 -g -O3 -Wall
EXECUTABLE=proj11.server

all: $(EXECUTABLE)

$(EXECUTABLE): proj11.server.c
	g++ $(CPPFLAGS) proj11.server.c -o $(EXECUTABLE)

clean: 
	rm -f *.o $(EXECUTABLE)