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
		//printf("<h1>Modulos</h1>\n");

		system("lsmod > lsmod.txt");

		printf("<p style=\"font-size:140%%; background-color:#04151EB8; margin:0; margin-top:10; color:white; font-family:verdana\"><b>");
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
   		token = strtok_r(NULL, "\n", &end_str); // No uso la primer fila
   		
   		int odd = 1;
   		while(token != NULL) {

   			if(odd%2 != 0){
   				printf("<p style=\"background-color:#0051ba99; margin:0\">");
   			}
   			else{
   				printf("<p style=\"background-color:#6881D88C; margin:0\">");
   			}

   			char *end_token;
   			char * token2;
   			token2 = strtok_r(token, " ", &end_token);
   			
   			char line[SIZE] = {""};
   			int index = 0;
   			while(token2 != NULL){
   				//printf("<span style=\"display:inline-block; width: 200;\">|%s|</span>", token2);
   				strcat(line, "<span style=\"display:inline-block; width: 200; margin-top: 5; margin-bottom: 5;\">|");
   				strcat(line, token2);
   				strcat(line, "|</span>");

   				token2 = strtok_r(NULL, " ", &end_token);
   				index++;
   			}

   			// Evita que se imprima basura al final 
   			if(index > 1){
   				printf("%s", line);
   			}

   			printf("</p>\n");
      		token = strtok_r(NULL, "\n", &end_str);

      		odd++;
   		}

   		/* Formulario de archivo */
   		printf("<form style=\"margin-top: 40; margin-left: 20\" method=\"POST\" enctype=\"multipart/form-data\" action=\"hello.cgi\">\n");
   		printf("<div style=\"margin: 10;\">\n");
   		printf("<label for=\"file\" style=\"margin-right: 30;\">Upload your own module:</label>\n");
   		printf("<input type=\"file\" id=\"file\" name=\"file\" />\n");
   		printf("</div>\n");
   		printf("<div style=\"margin: 10; margin-top: 20;\">\n");
   		printf("<button>Submit</button>\n");
   		printf("</div>\n");
   		printf("</form>");
   		

	printf("</body>\n");

	printf("</html>\n");

	return 1;
}