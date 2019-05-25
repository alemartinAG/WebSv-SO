#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 2048

bool checkUpload();

int main(void) {

	char buffer[SIZE];

	char *data;
	int uploaded = 0;
	data = getenv("QUERY_STRING");

	printf("Content-type: text/html; charset=UTF-8\n\n");
	printf("<html>\n");

	printf("<head>\n");
		printf("<title>Kernel Modules</title>\n");
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

   		//if(!checkUpload()){
	   		/* Formulario de archivo */

   			//if(data != NULL){
				if(sscanf(data,"m=%d", &uploaded) == 1 && uploaded == 1){
					printf("<form action=\"modhandler.cgi?delete=1\">\n");
					printf("<div style=\"margin: 10; margin-top: 20;\">\n");
			   		printf("<center><button>Remove Module</button></center>\n");
			   		printf("</div>\n");
			   		printf("</form>\n");
				}

				else{
					printf("<form style=\"margin-top: 40; margin-left: 20\" method=\"POST\" enctype=\"multipart/form-data\" action=\"modhandler.cgi\">\n");
			   		printf("<div style=\"margin: 10;\">\n");
			   		printf("<label for=\"file\" style=\"margin-right: 30;\">Upload your own module:</label>\n");
			   		printf("<input type=\"file\" id=\"file\" name=\"file\" accept=\".ko\"/>\n");
			   		printf("</div>\n");
			   		printf("<div style=\"margin: 10; margin-top: 20;\">\n");
			   		printf("<button>Submit</button>\n");
			   		printf("</div>\n");
			   		printf("</form>");
				}
			//}



   		//}
   		/*else{
   			printf("<div style=\"margin: 10; margin-top: 20;\">\n");
	   		printf("<button>Delete</button>\n");
	   		printf("</div>\n");
   		}*/

   		

	printf("</body>\n");

	printf("</html>\n");

	return 1;
}

bool checkUpload(){
	
	char *request_method;
	char *content_length;
	char *content_type;
	char *boundary, *pchar;
	char *pstart, *pend;

	int data_len, data_read, i,found;
	char *buffer; 

	request_method = getenv("REQUEST_METHOD");
	content_length = getenv("CONTENT_LENGTH");
	content_type = getenv("CONTENT_TYPE");

	if(request_method == NULL){
		printf("<center>Error: Unknown cgi-bin request</center>\n");
		return false;
	}

	/* Accediendo por primera vez */
	if(!strcmp(request_method, "GET")){
		return false;
	}

	/* This only handles POST */ 
	if(strcmp(request_method, "POST") != 0){
		printf("<center>Error: %s request method not supported</center>\n", request_method);
		return false;
	}

	//printf("<center>CONTENT_TYPE: %s</center>\n", content_type);

	/* The total length of what the client sent upstream */
    data_len = atoi(content_length);

    // Allocating memory for data
    buffer = (char *)malloc(data_len);

    // Reading file
    data_read = fread((void *)buffer, 1, data_len, stdin);
    printf("<p>Read %d bytes (of %d) from stdin</p>\n", data_read, data_len);

	/* Get the boundary marker for start/end of the data */
	pchar = strtok(buffer, "\r\n");
	boundary = strdup(buffer);

	char *datastart = "\r\n\r\n";

	/* Find the data start */
	pstart = strstr(buffer+strlen(boundary)+1, datastart);
	if(pstart == NULL){
	    printf("<p>ERROR - BAD DATA START</p>\n");
	    for(i=0; i<10; i++){
	          printf("%c ", *(buffer+strlen(boundary)+i));
	    }

	    free(boundary);
	    free(buffer);
	    return false;
	}

	/* Move the pointer past the end of headers marker */
	pstart+=strlen(datastart);


	/* Find the data end
	Can't use strstr here as the data may be binary */
	found = 0;
	
	for(pchar=pstart; pchar < buffer+data_len-strlen(boundary); pchar++){
	    if(memcmp(pchar, boundary, strlen(boundary)) == 0){
			found = 1;
			break;
	    }
	}

	if(found == 1){
	    pend = pchar;
	} 
	else{
	    printf("<p>ERROR - BAD DATA END</p>\n");
	    
	    free(boundary);
	    free(buffer);
	    return false;
	}

	/* Write the data to a file */
	FILE *fp;
	fp = fopen("module/umodule.ko", "wb");
	
	if(fp == NULL){
	    printf("<p>Error opening output file for write</p>\n");
	    
	    free(boundary); 
    	free(buffer);
	 	return false;
	} 
	else{
	    fwrite(pstart, 1, pend-pstart, fp);
	    fclose(fp);
	    printf("<p>Data Written to file</p>\n");

	    system("sudo insmod module/umodule.ko");
	}

    free(boundary); 
    free(buffer);

    return true;
}