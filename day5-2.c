#include <stdio.h>
#include <stdlib.h>

int cmp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 12;
    int w = 10;
    int sz = 20;
    int* a = calloc(sz, sizeof(int));
    int n = 0;
    int curr = 0, seat = 0;
    char line[lnsz];
    FILE* fp = fopen(argv[1], "r");
     
    if (!fp)
        return -2;

    while (fgets(line, lnsz, fp) != NULL) {
        curr = 0;
        
        for (int i = 0; i < w; i++) {
            switch (line[i]) {
            case 'B':
            case 'R':
                curr |= (1 << (9 - i));
                break;
            }
        }

        a[n] = curr;

        if (++n == sz) {
            sz *= 2;
            a = reallocarray(a, sz, sizeof(int));
        }
    }

    qsort(a, n, sizeof(int), cmp);

    for (int i = 1; i < n; i++) {
        //printf("%d\n", a[i]);
        if (a[i] != (a[i - 1] + 1)) {
            seat = a[i] - 1;
            break;
        }
    }

    fclose(fp);
    free(a);
    
    printf("%d\n", seat);
    
    return 0;
}
