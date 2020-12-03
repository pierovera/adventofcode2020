#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_WIDTH 32
#define SLOPES 5

int main(int argc, char** argv) {
    if (argc != 2)
	return -1;

    int sz = 20;
    
    char** a = calloc(sz, sizeof(char*));
    for (int i = 0; i < sz; i++)
	a[i] = calloc(LINE_WIDTH, sizeof(char));
    char buf[LINE_WIDTH + 1];
    FILE* fp = fopen(argv[1], "r");
    int n = 0;

    if (!fp)
	return -2;

    while (fgets(buf, LINE_WIDTH + 1, fp) != NULL) {
	sscanf(buf, "%s", a[n]);

	if (++n == sz) {
	    sz *= 2;
	    a = reallocarray(a, sz, sizeof(char*));
	    for (int i = n; i < sz; i++)
		a[i] = calloc(LINE_WIDTH, sizeof(char));
	}
    }	

    fclose(fp);

    int slopes[SLOPES][3] = {
	{1, 1, 0}, {3, 1, 0}, {5, 1, 0}, {7, 1, 0}, {1, 2, 0}
    };

    int col = 0;
    long res = 1;

    for (int k = 0; k < SLOPES; k++) {
	for (int i = slopes[k][1]; i < n; i += slopes[k][1]) {
	    col = (col + slopes[k][0]) % (LINE_WIDTH - 1);
	    if (a[i][col] == '#')
		slopes[k][2]++;
	}
	col = 0;
	res *= slopes[k][2];
    }

    printf("%ld\n", res);

    for (int i = 0; i < sz; i++)
	free(a[i]);
    free(a);
    
    return 0;
}
