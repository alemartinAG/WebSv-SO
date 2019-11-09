#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 3500
#define MAX_PATH 64

int main(void) {
	
	printf("Content-type: text/html; charset=UTF-8\n\n");
	printf("<html>\n");

	printf("<head>\n");
		printf("<title>System Info</title>\n");
	printf("</head>\n");

	printf("<body style=\"background-color:#fccece66;\">\n");
	
	/* Obtengo uptime */
	printf("<h4 style=\"background-color:#60a3f961;\"> -Uptime- </h4>\n");
	printf("<p>");
		FILE *fp;
		fp = fopen("/proc/uptime", "r");
		char buff[SIZE];
		fread(buff, SIZE, 1, fp);
		fclose(fp);

		long uptime;
		char *ptr;
		uptime = strtol(buff, &ptr, 10);

		memset(buff, '\0', SIZE-1);
		
	    sprintf(buff, "Uptime: %02ldD %02ld:%02ld:%02ld", (uptime / 60 / 60 / 24), (uptime / 60 / 60 % 24), (uptime / 60 % 60), (uptime % 60));

		printf("%s", buff);
	printf("</p>\n");

	/* Obtengo date */
	printf("<h4 style=\"background-color:#60a3f961;\"> -Date- </h4>\n");
	printf("<p>");
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		printf("%02d/%02d/%04d -- %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	printf("</ul>\n");

	/* Obtengo meminfo */
	printf("<h4 style=\"background-color:#60a3f961;\"> -Memoria- </h4>\n");
	printf("<p>");
		fp = fopen("/proc/meminfo", "r");
		fread(buff, SIZE, 1, fp);
		fclose(fp);

		char * token;
		token = strtok(buff, "\n");

		int index = 0;
		while(token != NULL && index < 7){
			printf("<li>");
			printf("%s", token);
			printf("</li>");

			token = strtok(NULL, "\n");
			index++;
		}

		
	printf("</ul>\n");

	/* Obtengo cpu */
	printf("<h4 style=\"background-color:#60a3f961;\"> -CPU- </h4>\n");
	printf("<p>");

	    system("ps -A --format %cpu > /home/ale/Documents/web-server/ps.txt");

	    /*char path[MAX_PATH];
		strcpy(path, getenv("HOME"));
		strcat(path, "Documents/web-server/ps.txt");*/
		
		//fp = fopen(path, "r");
		fp = fopen("/home/ale/Documents/web-server", "r");


		if(fp == NULL){
			printf("PORQUERIA\n");
		}
		else{
			memset(buff, '\0', SIZE);
			fread(buff, SIZE, 1, fp);
			fclose(fp);

			token = strtok(buff, "\n");
			float proceso, total;
			total = 0.0;
			while(token != NULL){

				if(sscanf(token,"%f", &proceso) == 1){
					total += proceso;
				}
				
				token = strtok(NULL, "\n");
			}

			printf("%% %.2f\n", total);

			//system("rm $HOME/Documents/web-server/ps.txt");
		}
	
	printf("</p>\n");
	

	printf("</body>\n");

	printf("</html>\n");

	return 1;
}
