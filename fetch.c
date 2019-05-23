#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "chtoli.h"

#define SIZE 2048

int main(void) {

	int year, day, hour;
	char *data;
	bool valid = true;

	printf("Content-type: text/html; charset=UTF-8\n\n");
	printf("<html>\n");

	data = getenv("QUERY_STRING");

	if(data == NULL){
  		printf("<P>Error! Error in passing data from form to script.");
	}
	else{
		// Chequeo que sean numeros
		if(sscanf(data,"ano=%d&dia=%d&hora=%d", &year, &day, &hour) != 3){
			valid = false;
		}

		//Chequeo límites
		if(day < 1 || day > 365 || hour < 0 || hour > 23){
			valid = false;
		}
	}
	
	if(!valid){
		printf("<head>\n");
		printf("<title>Error</title>\n");
		printf("</head>\n");

 	 	printf("<P>Error! Invalid Input\n");
 	 	printf("Possible Causes:\n");
 	 	printf("<ul>\n");
 	 	printf("<li>Data is not numeric</li>\n");
 	 	printf("<li>Day value exceeds 365</li>\n");
 	 	printf("<li>Day value is negative</li>\n");
 	 	printf("<li>Year value is smaller than 1900</li>\n");
 	 	printf("</ul>\n");
	}
 	else{
 		
 		printf("<head>\n");
		printf("<title>%d/%d - List</title>\n", year, day);
		printf("</head>\n");

		printf("<body>\n");

		printf("<h1>Archivos disponibles de Goes 16 en AWS</h1>\n");

		char * command;
		command = calloc(256, sizeof(char));
		strcpy(command, "echo nein");
		snprintf(command, 250, "aws s3 ls noaa-goes16/ABI-L2-CMIPF/%d/%03d/02/ > list.txt", year, day);
		system(command);

		free(command);

		char buffer[SIZE];

		FILE *fp;
		fp = fopen("list.txt", "r");
		fread(buffer, SIZE, 1, fp);
		fclose(fp);
		system("rm list.txt");

		//printf("<p>%s</p>\n", buffer);

		char * token;

		token = strtok(buffer, "\n");

		printf("<ul>\n");
		while(token != NULL){
			printf("<li style=\"margin-bottom:10; margin-top:10\">%s</li>\n", token);
			token = strtok(NULL, "\n");
		}
		printf("</ul>\n");

		//chunkToList(buffer, "\n", " ");

		printf("</body>\n");
 	}


	printf("</html>\n");

	return 1;
}