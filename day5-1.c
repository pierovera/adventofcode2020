#include <stdio.h>
#include <math.h>

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 12;
    int sz = 10;
    int max = 0,
        curr;
    int look = 1;
    char line[lnsz];
    FILE* fp = fopen(argv[1], "r");
     
    if (!fp)
        return -2;

    while (fgets(line, lnsz, fp) != NULL) {
        curr = 0;
        look = 1;
        
        for (int i = 0; i < sz && look; i++) {
            switch (line[i]) {
            case 'B':
            case 'R':
                curr |= (1 << (9 - i));
                break;
            case 'F':
            case 'L':
                if ((max & (((int)pow(2, i+1) - 1) << (9 - i))) > curr)
                    look = 0;
                break;
            }
        }

        if (look) max = curr;
    }

    fclose(fp);
    printf("%d\n", max);
    
    return 0;
}
