eseguibile: procedure.o main.o monitor.o
	@echo "Creazione file eseguibile..."
	@gcc -o eseguibile procedure.o main.o monitor.o

procedure.o: procedure.c monitor.h common.h
	@echo "compilo procedure..."
	@gcc -c procedure.c

monitor.o: monitor.c monitor.h
	@echo "compilo monitor..."
	@gcc -c monitor.c

main.o: main.c monitor.h common.h
	@echo "compilo il main..."
	@gcc -c main.c

clean:
	@rm *.o
	@rm eseguibile