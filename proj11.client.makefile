CPPFLAGS=-std=c++14 -g -O3 -Wall
EXECUTABLE=proj11.client

all: $(EXECUTABLE)

$(EXECUTABLE): proj11.client.c
	g++ $(CPPFLAGS) proj11.client.c -o $(EXECUTABLE)

clean: 
	rm -f *.o $(EXECUTABLE)