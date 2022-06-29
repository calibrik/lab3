all:
				clang main.c handle.c node.c control.c table.c
				valgrind --leak-check=full ./a.out
test:
				clang -DDEBUG main.c handle.c node.c control.c table.c
				valgrind ./a.out

