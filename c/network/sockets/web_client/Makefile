CC=clang
CFLAGS= -g

all: tcpc tcps

tcpc: tcpclient.c
	$(CC) $(CFLAGS) -o client tcpclient.c

tcps: tcpserver.c
	$(CC) $(CFLAGS) -o server tcpserver.c

clean:
	rm -rf *.dSYM server client
