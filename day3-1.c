#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_WIDTH 32

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

    int col = 0;
    int trees = 0;
    
    for (int i = 1; i < n; i++) {
        col = (col + 3) % (LINE_WIDTH - 1);
        if (a[i][col] == '#')
            trees++;
    }

    printf("%d\n", trees);

    for (int i = 0; i < sz; i++)
        free(a[i]);
    free(a);
    
    return 0;
}
