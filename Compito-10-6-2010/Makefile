meteo: procedure.o main.o monitor_hoare.h procedure.h monitor_hoare.o
	@gcc -o meteo procedure.o main.o monitor_hoare.o

procedure.o: procedure.c procedure.h
	@gcc -c procedure.c

main.o: main.c procedure.h
	@gcc -c main.c

monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	@gcc -c monitor_hoare.c

clean:
	@rm *.o
	@rm meteo