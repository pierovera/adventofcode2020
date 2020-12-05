#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 12;
    int w = 10;
    int curr = 0, seat = 0,
        min = INT_MAX, max = 0,
        ids = 0;
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

        max = max > curr ? max : curr;
        min = min < curr ? min : curr;
        ids += curr;
    }

    fclose(fp);

    seat = ((max * (max + 1) - min * (min - 1)) >> 1) - ids;
    
    printf("%d\n", seat);
    
    return 0;
}
