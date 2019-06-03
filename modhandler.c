#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 2048

#define ERROR 3
#define NOT 2
#define OK  1

int checkUpload();

int main(void) {

	char buffer[SIZE];

	char *data;
	int remove = 0;
	data = getenv("QUERY_STRING");

	printf("Content-type: text/html; charset=UTF-8\n\n");
	printf("<html>\n");

	printf("<head>\n");
		printf("<title>Kernel Modules</title>\n");
	printf("</head>\n");

	printf("<body>\n");
	
	
	/* Chequeo si se subio el modulo o si lo tengo que remover */
	int result = checkUpload();
	if(result == OK){

		printf("<center><p style=\"color:green; font-size:x-large; font-family:arial\">Module Succesfully Uploaded</p></center>\n");

		/* Vuelvo a la lista de modulos */
		printf("<div style=\"margin: 10; margin-top: 20;\">\n");
   		printf("<center><a href=\"mods.cgi?m=1\"><button>Accept</button></a></center>\n");
   		printf("</div>\n");

	}
	else{

		if(result == NOT && sscanf(data,"delete=%d", &remove) == 1 && remove == 1){
			system("sudo rmmod decrypter");
			printf("<center><p style=\"color:red; font-size:x-large; font-family:arial\">Module Succesfully Removed</p></center>\n");
		}
		
		/* Vuelvo a la lista de modulos */
		printf("<div style=\"margin: 10; margin-top: 20;\">\n");
   		printf("<center><a href=\"mods.cgi\"><button>Accept</button></a></center>\n");
   		printf("</div>\n");
	}

	printf("</body>\n");

	printf("</html>\n");

	return 1;
}

int checkUpload(){
	
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
		return ERROR;
	}

	/* This only handles POST */ 
	if(strcmp(request_method, "POST") != 0){
		//printf("<center>Error: %s request method not supported</center>\n", request_method);
		return NOT;
	}

	//printf("<center>CONTENT_TYPE: %s</center>\n", content_type);

	/* The total length of what the client sent upstream */
    data_len = atoi(content_length);

    // Allocating memory for data
    buffer = (char *)malloc(data_len);

    // Reading file
    data_read = fread((void *)buffer, 1, data_len, stdin);
    //printf("<p>Read %d bytes (of %d) from stdin</p>\n", data_read, data_len);

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
	    return ERROR;
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
	    return ERROR;
	}

	/* Write the data to a file */
	FILE *fp;
	fp = fopen("module/umodule.ko", "wb");
	
	if(fp == NULL){
	    printf("<p>Error opening output file for write</p>\n");
	    
	    free(boundary); 
    	free(buffer);
	 	return ERROR;
	} 
    
    fwrite(pstart, 1, pend-pstart, fp);
    fclose(fp);

    memset(buffer, 0, data_len);
    system("sudo insmod module/umodule.ko 2> error.txt");
    fp = fopen("error.txt", "r");
    fread(buffer, sizeof(char), data_len, fp);

    if(strlen(buffer) > 1){
    	printf("<center><p style=\"color:red; font-size:x-large; font-family:arial\">Error Cargando Modulo</p></center>\n");
    	printf("<center><p style=\"color:red; \">%s</p></center>\n", buffer);
    	return ERROR;
    }

    free(boundary); 
    free(buffer);

    return OK;
}