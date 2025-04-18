all: start server client

start: main.o procedure.h
	@gcc -o start main.o -pthread

client: client.o procedure.h
	@gcc -o client client.o -pthread

server: server.o procedure.h
	@gcc -o server server.o -pthread

main.o: main.c procedure.h
	@gcc -c main.c

client.o: client.c procedure.h
	@gcc -c client.c

server.o: server.c procedure.h
	@gcc -c server.c


clean:
	@rm *.o
	@rm start
	@rm client
	@rm server
	