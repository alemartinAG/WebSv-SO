#include <stdio.h>
#include <stdlib.h>

int main(void) {
	
	printf("Content-type: text/html; charset=UTF-8\n\n");
	printf("<html>\n");

	printf("<head>\n");
		printf("<title>Archive</title>\n");
	printf("</head>\n");

	printf("<body>\n");
		printf("<h1>Obtener Archivo</h1>\n");

	printf("<form action=\"fetch.cgi\">\n");
		printf("<div><label>Año: <input name=\"ano\" size=\"5\"></label></div>\n");
		printf("<div><label>Día: <input name=\"dia\" size=\"5\"></label></div>\n");
		printf("<div><label>Hora: <input name=\"hora\" size=\"5\"></label></div>\n");
		printf("<div><input type=\"submit\" value=\"Buscar\"></div>\n");
	printf("</form>\n");

	printf("</body>\n");

	printf("</html>\n");

	return 1;
}