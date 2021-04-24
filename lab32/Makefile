all:
	g++ server.c -Wall -laio -lsocket -lnsl -o server
	g++ client.c -Wall -lsocket -lnsl -o client

linux:
	g++ server.c -Wall -lrt -o server
	g++ client.c -Wall -lrt -o client