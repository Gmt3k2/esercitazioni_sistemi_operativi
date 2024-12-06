
chat: main.o procedure.o common.h
	@echo "Generating exe..."
	@gcc -o chat main.o procedure.o

main.o: main.c common.h
	@echo "Compiling main..."
	@gcc -c main.c
procedure.o: procedure.c common.h
	@echo "Compiling procedure..."
	@gcc -c procedure.c


clean:
	@rm chat
	@rm *.o