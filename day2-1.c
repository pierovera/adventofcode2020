#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PASS 30

struct Pass {
    int min;
    int max;
    char c;
    char pass[MAX_PASS];
};

int main(int argc, char** argv) {
    if (argc != 2)
	return -1;

    int sz = 20;
    int bfsz = 40;
    struct Pass* a = calloc(sz, sizeof(struct Pass));
    char* strbuf = calloc(MAX_PASS, sizeof(char));
    FILE* fp = fopen(argv[1], "r");
    char buf[bfsz];
    int n = 0;

    if (!fp)
	return -2;

    while (fgets(buf, bfsz, fp) != NULL) {
	sscanf(buf, "%d-%d %c: %s", &a[n].min, &a[n].max, &a[n].c, strbuf);
	strncpy(a[n].pass, strbuf, MAX_PASS - 1);

	//printf("%d - %d (%c): %s\n", a[n].min, a[n].max, a[n].c, a[n].pass);

	if (++n == sz) {
	    sz *= 2;
	    a = reallocarray(a, sz, sizeof(struct Pass));
	}
    }

    free(strbuf);

    fclose(fp);

    free(a);
    
    return 0;
}
