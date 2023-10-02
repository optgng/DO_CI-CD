CC = gcc
CFLAGS = -Wall -Werror -Wextra -IIncludes -IDependencies -g

all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(CFLAGS) s21_cat.o  -o  s21_cat 
	rm -rf *.o

s21_cat.o: s21_cat.c 
	$(CC) $(CFLAGS) -c s21_cat.c

style: 
	clang-format -style=google -n *.c
	clang-format -style=google -n *.h
	clang-format -style=google -i *.c
	clang-format -style=google -i *.h

clean:
	rm -rf *.o s21_cat

rebuild:
	$(MAKE) clean
	$(MAKE) all
  