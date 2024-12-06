eseguibile: procedure.o main.o monitor_signal_continue.o monitor_signal_continue.h procedure.h
	@echo "Generating exe..."
	@gcc -o eseguibile procedure.o main.o monitor_signal_continue.o

procedure.o: procedure.c monitor_signal_continue.h procedure.h
	@echo "Compiling procedure..."
	@gcc -c procedure.c

main.o: main.c monitor_signal_continue.h procedure.h
	@echo "Compiling main..."
	@gcc -c main.c

monitor_signal_continue.o: procedure.c monitor_signal_continue.h
	@echo "Compiling monitor signal continue..."
	@gcc -c monitor_signal_continue.c

clean:
	@echo "Removing compiled files..."
	@rm eseguibile
	@rm *.o