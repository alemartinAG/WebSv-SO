CC=gcc
CFLAGS=-Werror​ -Wall -pedantic

all:
	$(CC) hello.c -o hello.cgi
	$(CC) archive.c -o archive.cgi
	$(CC) fetch.c -o fetch.cgi
	$(CC) mods.c -o mods.cgi
clean:
	rm *.cgi
cppcheck:
	cppcheck --enable=all  --suppress=missingIncludeSystem *.c 2>cpp_check.txt
