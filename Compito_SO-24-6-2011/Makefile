eseguibile: monitor_hoare.o procedure.o main.o
	@gcc -o eseguibile monitor_hoare.o procedure.o main.o

monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	@gcc -c monitor_hoare.c

procedure.o: procedure.c monitor_hoare.h procedure.h
	@gcc -c procedure.c

main.o: main.c monitor_hoare.h procedure.h
	@gcc -c main.c

clean:
	@rm eseguibile
	@rm *.o