all: main client_asta server_asta 

main: main.c
	gcc -o main main.c 

client_asta: client_asta.c header.h
	gcc -o client_asta -pthread client_asta.c

server_asta: server_asta.o monitor_hoare.o
	gcc -o server_asta server_asta.o monitor_hoare.o

server_asta.o: server_asta.c header.h
	gcc -c server_asta.c

monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	gcc -c monitor_hoare.c	

clean:
	rm client_asta
	rm server_asta
	rm main
	rm *.o