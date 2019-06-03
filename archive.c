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

	printf("<form style=\"margin-left:25px\" action=\"fetch.cgi\">\n");
		printf("<div style=\"margin-top:15px\"><label>Año: <input name=\"ano\" size=\"5\"></label></div>\n");
		printf("<div style=\"margin-top:15px\"><label>Día: <input name=\"dia\" size=\"5\"></label></div>\n");
		printf("<div style=\"margin-top:15px\"><input type=\"submit\" value=\"Buscar\"></div>\n");
	printf("</form>\n");

	printf("</body>\n");

	printf("</html>\n");

	return 1;
}