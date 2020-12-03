#include <stdio.h>
#include <stdlib.h>

int cmp(const void* a, const void* b) {
    return (*(short*)a - *(short*)b);
}

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int sz = 20;
    int bfsz = 16;
    short* a = calloc(sz, sizeof(short));
    FILE* fp = fopen(argv[1], "r");
    char buf[bfsz];
    int n = 0;

    if (!fp)
        return -2;

    while (fgets(buf, bfsz, fp) != NULL) {
        sscanf(buf, "%hd", &a[n]);

        if (++n == sz) {
            sz *= 2;
            a = reallocarray(a, sz, sizeof(short));
        }
    }

    fclose(fp);
    qsort(a, n, sizeof(short), cmp);

    int j, r;
    
    for (int i = 0; i < n; i++) {
        j = n - 1;
        do {
            r = a[j] + a[i];
            j--;
        } while (r > 2020 && j > i);

        if (r == 2020) {
            r = (int) a[++j] * a[i];
            free(a);
            printf("%d\n", r);
            return 0;
        }
    }

    free(a);
    
    return -3;
}
