CC=gcc
CFLAGS=-Werror -Wall -pedantic

all:
	$(CC) $(CFLAGS) info.c -o info.cgi
	$(CC) $(CFLAGS) archive.c -o archive.cgi
	$(CC) $(CFLAGS) fetch.c -o fetch.cgi
	$(CC) $(CFLAGS) mods.c -o mods.cgi
	$(CC) $(CFLAGS) modhandler.c -o modhandler.cgi
clean:
	rm *.cgi
cppcheck:
	cppcheck --enable=all  --suppress=missingIncludeSystem *.c 2>cpp_check.txt
