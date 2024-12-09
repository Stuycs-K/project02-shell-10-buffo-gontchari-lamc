run: shell
	@./shell
compile shell: shell.o
	@gcc -o shell shell.o
shell.o: shell.c
	@gcc -c shell.c
clean:
	@rm -rf *.o
	@rm -rf shell
