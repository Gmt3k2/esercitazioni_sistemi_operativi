all: main.o prodcons.o
	gcc -lpthread -o start  main.o prodcons.o

main.o: main.c prodcons.h
	gcc -lpthread -c -o main.o  main.c

prodcons.o: prodcons.c prodcons.h
	gcc -lpthread -c -o prodcons.o  prodcons.c

clean:
	rm -f *.o
	rm -f start
