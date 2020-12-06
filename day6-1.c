#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 30,
        sz = 30;
    int len;
    int sum = 0;
    uint32_t cur = 0;
    char buf[sz], line[lnsz];
    FILE* fp = fopen(argv[1], "r");
     
    if (!fp)
        return -2;

    while (fgets(line, lnsz, fp) != NULL) {        
        if (line[0] == '\n') {
            while (cur) {
                sum += cur & 1;
                cur >>= 1;
            }
            continue;
        }

        sscanf(line, "%s", buf);

        len = strlen(buf);
        for (int i = 0; i < len; i++) 
            cur |= 1 << (buf[i] - 'a');
    }

    while (cur) {
        sum += cur & 1;
        cur >>= 1;
    }
    
    fclose(fp);
    printf("%d\n", sum);
    
    return 0;
}
