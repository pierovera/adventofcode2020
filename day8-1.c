#include <stdio.h>
#include <stdlib.h>

struct Ins {
    char dir[3];
    int n;
    int exec;
};

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 10;
    int sz = 20;
    int n = 0;
    int acc = 0;
    struct Ins* a = calloc(sz, sizeof(struct Ins));
    char line[lnsz];
    FILE* fp = fopen(argv[1], "r");
     
    if (!fp)
        return -2;

    while (fgets(line, lnsz, fp) != NULL) {
        sscanf(line, "%3s", a[n].dir);
        sscanf(line + 4, "%d", &a[n].n);
        a[n].exec = 0;
        
        if (++n == sz) {
            sz *= 2;
            a = reallocarray(a, sz, sizeof(struct Ins));
        }
    }
    
    fclose(fp);

    int i = 0;
    while (!a[i].exec) {
        a[i].exec = 1;
        
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

    free(a);
    printf("%d\n", acc);
    
    return 0;
}
