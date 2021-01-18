All: tcpip client
CC=g++
CFLAGS= 
LFLAGS= -lpthread -lboost_system
tcpip:  main.o
	$(CC) main.o $(LFLAGS) -o tcpip

nain.o:  main.cpp
	$(CC) -c main.cpp -o main.o
client: client.o
	$(CC)  client.o $(LFLAGS) -o client
client.o: client.cpp
	$(CC) -c $(CFLAGS) client.cpp -o client.o
clean:
	rm *.o tcpip client