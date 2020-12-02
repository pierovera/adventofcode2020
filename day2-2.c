#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

	if (++n == sz) {
	    sz *= 2;
	    a = reallocarray(a, sz, sizeof(struct Pass));
	}
    }

    free(strbuf);
    fclose(fp);

    int valid = 0;
    bool b, c;
    
    for (int i = 0; i < n; i++) {
	b = a[i].pass[ a[i].min - 1 ] == a[i].c;
	c = a[i].pass[ a[i].max - 1 ] == a[i].c;
	
	if ((c && !b) || (!c && b))
	    valid++;
    }

    printf("%d\n", valid);
    free(a);
    
    return 0;
}
