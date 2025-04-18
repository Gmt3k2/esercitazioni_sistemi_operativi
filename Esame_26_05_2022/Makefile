main: main.o procedure.o procedure.h monitor_hoare.h monitor_hoare.o
	@gcc -o main main.o procedure.o monitor_hoare.o

main.o: main.c procedure.h monitor_hoare.h 
	@gcc -c main.c

procedure.o: procedure.c procedure.h monitor_hoare.h
	@gcc -c procedure.c

monitor_hoare.o: monitor_hoare.h
	@gcc -c monitor_hoare.c

clean:
	rm *.o
	rm main