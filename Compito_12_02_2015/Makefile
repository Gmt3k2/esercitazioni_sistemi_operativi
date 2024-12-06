eseguibile: main.o procedure.o
	@echo "Generating exe..."
	@gcc -pthread -o eseguibile main.o procedure.o

procedure.o: procedure.c procedure.h
	@echo "Compiling procedure..."
	@gcc -c procedure.c

main.o: main.c procedure.h
	@echo "Compiling main..."
	@gcc -c main.c

clean:
	@rm eseguibile
	@rm *.o