All: tcpip client sslclient sslserver
CC=g++
CFLAGS= -I/usr/src/beast 
LFLAGS= -lpthread -lboost_system -lssl -lcrypto
tcpip:  main.o
	$(CC) main.o $(LFLAGS) -o tcpip

nain.o:  main.cpp
	$(CC) -c main.cpp -o main.o
client: client.o
	$(CC)  client.o $(LFLAGS) -o client
client.o: client.cpp
	$(CC) -c $(CFLAGS) client.cpp -o client.o
clean:
	rm *.o tcpip client sslclient sslserver
sslclient.o: sslclient.cpp
	$(CC) -c $(CFLAGS) sslclient.cpp -o sslclient.o
sslclient: sslclient.o
	$(CC) $(LFLAGS) sslclient.o -o sslclient
sslserver.o: sslserver.cpp
	$(CC) -c $(CFLAGS) sslserver.cpp -o sslserver.o
sslserver: sslserver.o
	$(CC) $(LFLAGS) sslserver.o -o sslserver