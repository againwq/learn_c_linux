all: client server

client: client.o
	gcc -o client client.o
server:  server.o
	gcc -o server server.o
server.o:
	gcc -c server.c -o server.o
client.o:
	gcc -c client.c -o client.o
clean:
	rm -f client server *.o