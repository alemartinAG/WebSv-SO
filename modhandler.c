#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 128

#define ERROR 3
#define NOT 2
#define OK  1

#define MOD_FILE "module/umodule.ko"
#define NAM_FILE "module/name.txt"
#define ERR_FILE "error.txt"

int checkUpload();
char * getModName();

int main(void) {

	char * data = malloc(SIZE);
	char * sysCmd = malloc(SIZE);
	
	int remove = 0;
	
	if(getenv("QUERY_STRING") != NULL){
		strcpy(data, getenv("QUERY_STRING"));
	}

	printf("Content-type: text/html; charset=UTF-8\n\n");
	printf("<html>\n");

	printf("<head>\n");
		printf("<title>Kernel Modules</title>\n");
	printf("</head>\n");

	printf("<body>\n");
	
	
	/* Chequeo si se subio el modulo o si lo tengo que remover */
	int result = checkUpload();
	if(result == OK){

		char * module_name = getModName();

		printf("<center><p style=\"color:green; font-size:x-large; font-family:arial\">Module \' %s\' Succesfully Uploaded</p></center>\n", module_name);

		free(sysCmd);

		/* Vuelvo a la lista de modulos */
		printf("<div style=\"margin: 10; margin-top: 20;\">\n");
   		printf("<center><a href=\"mods.cgi?m=1\"><button>Accept</button></a></center>\n");
   		printf("</div>\n");

	}
	else{

		if(result == NOT && sscanf(data,"delete=%d", &remove) == 1 && remove == 1){

			char * module_name = getModName();
			sprintf(sysCmd, "sudo rmmod %s", module_name);

			system(sysCmd);

			printf("<center><p style=\"color:red; font-size:x-large; font-family:arial\">Module: \' %s\' Succesfully Removed</p></center>\n", module_name);

			free(module_name);
			free(sysCmd);
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

char * getModName(){

	char * mod_name = malloc(SIZE);
	char * buffer = malloc(SIZE);

	FILE *fp;
	fp = fopen(NAM_FILE, "r");
	fread(buffer, sizeof(char), SIZE, fp);
	fclose(fp);

	char *token;
	token = strtok(buffer, " ");

	while(token != NULL){
		strcpy(mod_name, token);
		token = strtok(NULL, " ");
	}

	free(buffer);

	return mod_name;

}

int checkUpload(){
	
	char *request_method;
	char *content_length;
	char *boundary, *pchar;
	char *pstart, *pend;

	int data_len,found;
	char *buffer; 

	char * sysCmd = malloc(SIZE);


	request_method = getenv("REQUEST_METHOD");
	content_length = getenv("CONTENT_LENGTH");

	if(request_method == NULL){
		printf("<center>Error: Unknown cgi-bin request</center>\n");
		return ERROR;
	}

	/* This only handles POST */
	if(strcmp(request_method, "POST") != 0){
		return NOT;
	}

	/* The total length of what the client sent upstream */
    data_len = atoi(content_length);

    // Allocating memory for data
    buffer = (char *)malloc(data_len);

    // Reading file
    fread((void *)buffer, 1, data_len, stdin);

	/* Get the boundary marker for start/end of the data */
	pchar = strtok(buffer, "\r\n");
	boundary = strdup(buffer);

	char *datastart = "\r\n\r\n";

	/* Find the data start */
	pstart = strstr(buffer+strlen(boundary)+1, datastart);
	if(pstart == NULL){
	    printf("<p>ERROR - BAD DATA START</p>\n");
	    for(int i=0; i<10; i++){
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
	fp = fopen(MOD_FILE, "wb");
	
	if(fp == NULL){
	    printf("<p>Error opening output file</p>\n");
	    
	    free(boundary); 
    	free(buffer);
	 	return ERROR;
	} 
    
    fwrite(pstart, 1, pend-pstart, fp);
    fclose(fp);

    memset(buffer, 0, data_len);

    sprintf(sysCmd, "sudo insmod %s 2> %s", MOD_FILE, ERR_FILE);

    system(sysCmd);
    
    fp = fopen(ERR_FILE, "r");
    fread(buffer, sizeof(char), data_len, fp);
    fclose(fp);

    if(strlen(buffer) > 1){
    	printf("<center><p style=\"color:red; font-size:x-large; font-family:arial\">Error Cargando Modulo</p></center>\n");
    	printf("<center><p style=\"color:red; \">%s</p></center>\n", buffer);
    	return ERROR;
    }

    sprintf(sysCmd, "rm %s", ERR_FILE);
    system(sysCmd);

    sprintf(sysCmd, "modinfo %s | awk \'/^name/{print $0}\' > %s", MOD_FILE, NAM_FILE);
    system(sysCmd);


    free(boundary); 
    free(buffer);
    free(sysCmd);

    return OK;
}