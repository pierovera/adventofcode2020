#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYR 1883362
#define IYR 1883369
#define EYR 1883365
#define HGT 1913832
#define HCL 1782248
#define ECL 1782245
#define PID 1651952
#define CID 1651939

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 90;
    int read = 0;
    int cnt = 0;
    int valid = 0;
    int sz = 20;
    int tmp;
    unsigned long hash;
    char key[sz], val[sz], line[lnsz];
    FILE* fp = fopen(argv[1], "r");
     
    if (!fp)
        return -2;

    while (fgets(line, lnsz, fp) != NULL) {
        read = 0;
        
        if (line[0] == '\n') {
            if ((cnt & 0b01111111) == 0b01111111) valid++;
            cnt = 0;
            continue;
        }

        while (sscanf(line+read, "%[^:]:%s ", key, val) > 0) {
            tmp = strlen(key);
            read += tmp + strlen(val) + 2;
            if (tmp != 3) continue;

            hash = 0;
            for (int i = 0; i < 3; i++)
                hash += ( key[i] << (7*i) );

            switch (hash) {
            case BYR:
                cnt |= 0b00000001;
                break;
            case IYR:
                cnt |= 0b00000010;
                break;
            case EYR:
                cnt |= 0b00000100;
                break;
            case HGT:
                cnt |= 0b00001000;
                break;
            case HCL:
                cnt |= 0b00010000;
                break;
            case ECL:
                cnt |= 0b00100000;
                break;
            case PID:
                cnt |= 0b01000000;
                break;
            case CID:
                cnt |= 0b10000000;
                break;
            }
        }
    }

    if ((cnt & 0b01111111) == 0b01111111) valid++;

    fclose(fp);
    printf("%d\n", valid);
    
    return 0;
}
