#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 3500
#define MAX_PATH 64

int main(void) {

char path[MAX_PATH];
strcpy(path, getenv("PWD"));
strcat(path, "/ps.txt");

printf("%s\n", path);

}