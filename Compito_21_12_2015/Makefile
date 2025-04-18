main: main.o procedure.h procedure.o
	gcc -o main main.o procedure.o -lpthread

main.o: main.c procedure.c
	gcc -c main.c

procedure.o: procedure.h procedure.c
	gcc -c procedure.c

clean:
	rm *.o
	rm main