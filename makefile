all:
				clang main.c handle.c node.c control.c table.c
				valgrind --leak-check=full ./a.out
3a:
				clang -DLAB3A main.c handle.c node.c control.c table.c
				valgrind ./a.out

3b:
				clang -DLAB3B main.c handle.c node.c control.c table.c
				valgrind ./a.out
