#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BYR 1883362
#define IYR 1883369
#define EYR 1883365
#define HGT 1913832
#define HCL 1782248
#define ECL 1782245
#define PID 1651952
#define CID 1651939

#define AMB 1619681
#define BLU 1930850
#define BRN 1816930
#define GRY 1997159
#define GRN 1816935
#define HZL 1785192
#define OTH 1718895

int checkEye(char* val) {
    if (strlen(val) != 3)
        return 0;   

    unsigned int hash = 0;

    for (int i = 0; i < 3; i++)
        hash += ( val[i] << (7*i) );

    switch (hash) {
    case AMB:
    case BLU:
    case BRN:
    case GRY:
    case GRN:
    case HZL:
    case OTH:
        return 1;
        break;
    }
    
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 90;
    int read = 0;
    int cnt = 0;
    int valid = 0;
    int sz = 20;
    int tmp;
    unsigned int hash;
    char key[sz], val[sz], line[lnsz], valS[sz];
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
                sscanf(val, "%d", &tmp);
                if (tmp >= 1920 && tmp <= 2002)
                    cnt |= 0b00000001;
                break;
                
            case IYR:
                sscanf(val, "%d", &tmp);
                if (tmp >= 2010 && tmp <= 2020)
                    cnt |= 0b00000010;
                break;
                
            case EYR:
                sscanf(val, "%d", &tmp);
                if (tmp >= 2020 && tmp <= 2030)
                    cnt |= 0b00000100;
                break;
                
            case HGT:
                sscanf(val, "%d%s", &tmp, valS);
                if ((!(strncmp(valS, "cm", sz)) && tmp >= 150 && tmp <= 193) ||
                    (!(strncmp(valS, "in", sz)) && tmp >= 59 && tmp <= 76))
                    cnt |= 0b00001000;
                break;
                
            case HCL:
                sscanf(val, "%s", valS);
                tmp = strlen(valS);
                if (tmp != 7) break;
                for (int i = 1; i < tmp; i++) {
                    if (!(isxdigit(valS[i]))) break;
                }
                    cnt |= 0b00010000;
                break;
                
            case ECL:
                if (checkEye(val))
                    cnt |= 0b00100000;
                break;
                
            case PID:
                sscanf(val, "%s", valS);
                tmp = strlen(valS);
                if (tmp != 9) break;
                for (int i = 0; i < tmp; i++) {
                    if (!(isdigit(valS[i]))) break;
                }
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
