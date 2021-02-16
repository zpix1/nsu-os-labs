#include <stdlib.h>
int main() {
	char* a = malloc(1024 * 1024 * 1024);
	printf("malloc\n");
	scanf("%s", a);
	free(a);
	printf("free\n");
	scanf("%s", a);
	a = malloc(1024 * 1024 * 1024);
	printf("%.10s", a);
	freezero(a, 1024 * 1024 * 1024);
	printf("freezero\n");
	printf("%p", *a);
	scanf("%s", a);
	return 0;
}
