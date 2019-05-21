#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
	
	printf("Content-type: text/html\n\n");
	printf("<html>\n");

	printf("<head>\n");
		printf("<title>Hello</title>\n");
	printf("</head>\n");

	printf("<body>\n");
		printf("<h1>Goodbye Cruel World</h1>\n");
		printf("<h3>This is some kind of test</h3>\n");
	
	/* Obtengo uptime */
	printf("<h4> -Uptime- </h4>\n");
	printf("<p>");
		FILE *fp;
		fp = fopen("/proc/uptime", "r");
		char buff[3500];
		fread(buff, 3500, 1, fp);
		fclose(fp);

		printf("%s", buff);
	printf("</p>\n");

	/* Obtengo date */
	printf("<h4> -Date- </h4>\n");
	printf("<p>");
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		printf("%02d/%02d/%04d -- %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
	printf("</p>\n");

	/* Obtengo meminfo */
	printf("<h4> -Memoria- </h4>\n");
	printf("<p>");
		fp = fopen("/proc/meminfo", "r");
		fread(buff, 3500, 1, fp);
		fclose(fp);

		printf("%s", buff);
	printf("</p>\n");

	/* Obtengo meminfo */
	printf("<h4> -CPU- </h4>\n");
	printf("<p>");
		printf("CPU USAGE ACA\n");
	printf("</p>\n");
	

	printf("</body>\n");

	printf("</html>\n");

	return 1;
}