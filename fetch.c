#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void) {

	int year, day;
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
		if(sscanf(data,"ano=%d&dia=%d",&year,&day)!=2){
			valid = false;
		}

		//Chequeo límites
		if(day < 0 || day > 365 || year < 1900){
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
	
		/* Lista Ordenada */
		printf("<ol>\n");

			for(int i=0; i<10; i++){
				printf("<li>%d-%d-archivo</li>\n", year, day);
			}

		printf("</ol>\n");

		printf("</body>\n");
 	}


	printf("</html>\n");

	return 1;
}