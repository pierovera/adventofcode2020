#include <stdio.h>
#include <stdlib.h>

struct Ins {
    char dir[3];
    int n;
};

int execP(struct Ins* a, int n) {
    int acc = 0;
    int exec[n];

    for (int i = 0; i < n; i++)
        exec[i] = 0;

    int i = 0;
    while (!exec[i]) {
        exec[i] = 1;
        switch (a[i].dir[0]) {
        case 'a':
            acc += a[i].n;
        case 'n':
            i++;
            break;
        case 'j':
            i += a[i].n;
            break;
        }

        if (i == n)
            return acc;
    }
    
    return -1;
}

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 10;
    int sz = 20;
    int n = 0;
    int acc = -1;
    struct Ins* a = calloc(sz, sizeof(struct Ins));
    char line[lnsz];
    FILE* fp = fopen(argv[1], "r");
     
    if (!fp)
        return -2;

    while (fgets(line, lnsz, fp) != NULL) {
        sscanf(line, "%3s", a[n].dir);
        sscanf(line + 4, "%d", &a[n].n);
        
        if (++n == sz) {
            sz *= 2;
            a = reallocarray(a, sz, sizeof(struct Ins));
        }
    }
    
    fclose(fp);

    for (int i = 0; i < n; i++) {
        switch (a[i].dir[0]) {
        case 'a':
            continue;
        case 'j':
            a[i].dir[0] = 'n';
            acc = execP(a, n);
            a[i].dir[0] = 'j';
            break;
        case 'n':
            a[i].dir[0] = 'j';
            acc = execP(a, n);
            a[i].dir[0] = 'n';
            break;
        }

        if (acc > 0) break;
    }

    free(a);
    printf("%d\n", acc);
    
    return 0;
}
