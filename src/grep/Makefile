CC = gcc
CFLAGS = -Wall -Werror -Wextra -IIncludes -IDependencies -g

all: s21_grep

s21_grep: s21_grep.o
	$(CC) $(CFLAGS) s21_grep.o  -o  s21_grep
	rm -rf *.o

s21_grep.o: s21_grep.c 
	$(CC) $(CFLAGS) -c s21_grep.c

style: 
	clang-format -style=google -n *.c
	clang-format -style=google -n *.h

clean:
	rm -rf *.o s21_grep

rebuild:
	$(MAKE) clean
	$(MAKE) all
  