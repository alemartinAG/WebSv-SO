#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 2048

int main(void) {
	
	char buffer[SIZE];

	printf("Content-type: text/html; charset=UTF-8\n\n");
	printf("<html>\n");

	printf("<head>\n");
		printf("<title>Modules</title>\n");
	printf("</head>\n");

	printf("<body>\n");
		printf("<h1>Modulos</h1>\n");

		system("lsmod > lsmod.txt");

		printf("<p style=\"font-size:110%%;\"><b>");
		printf("<span style=\"display:inline-block; width: 200;\">Module</span>");
		printf("<span style=\"display:inline-block; width: 200;\">Size</span>");
		printf("<span style=\"display:inline-block; width: 200;\">Used</span>");
		printf("<span style=\"display:inline-block; width: 200;\">By</span>");
		printf("</b></p>");

		FILE *fp;
		fp = fopen("lsmod.txt", "r");
		fread(buffer, SIZE, 1, fp);
		fclose(fp);
		system("rm lsmod.txt");

		char * end_str;
		char * token;
   		
   		token = strtok_r(buffer, "\n", &end_str);

   		while(token != NULL) {

   			printf("<p>");

   			char *end_token;
   			char *token2 = strtok_r(token, " ", &end_token);
   			token2 = strtok_r(NULL, " ", &end_token);
   			while(token2 != NULL){
   				printf("<span style=\"display:inline-block; width: 200;\">|%s|</span>", token2);
   				token2 = strtok_r(NULL, " ", &end_token);
   			}

   			printf("</p>\n");
      		//printf("<p>%s</p>", token);
      		token = strtok_r(NULL, "\n", &end_str);
   		}

	printf("</form>\n");

	printf("</body>\n");

	printf("</html>\n");

	return 1;
}