#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    struct Node** p;
    int* cn;
    struct Node** c;
    size_t sz;
    int np;
    int nc;
    int cp;
    int cc;
    char* str;
};

struct Bucket {
    struct Node* data;
    uint32_t hash;
    struct Bucket* next;
};

struct Table {
    struct Bucket** a;
    int na;
};

struct Node* mkNode(char* str, size_t sz) {
    struct Node* node = calloc(1, sizeof(struct Node));
    node->str = str;
    node->sz = sz;
    node->np = node->nc = 4;
    node->cp = node->cc = 0;
    node->p = calloc(4, sizeof(struct Node*));
    node->c = calloc(4, sizeof(struct Node*));
    node->cn = calloc(4, sizeof(int));

    return node;
}

struct Bucket* mkBucket(struct Node* node, uint32_t hash) {
    struct Bucket* bucket = calloc(1, sizeof(struct Bucket));
    bucket->data = node;
    bucket->hash = hash;
    bucket->next = NULL;

    return bucket;
}

int insert(struct Table* tbl, struct Node* node, uint32_t hash) {
    uint32_t pos = hash % tbl->na;
    struct Bucket* b = tbl->a[pos];
    struct Bucket* new = mkBucket(node, hash);

    if (b == NULL) {
        tbl->a[pos] = new;
        return 0;
    }

    int res = 1;
    while(b->next != NULL) {
        b = b->next;
        res++;
    }

    b->next = new;
    return res;
}

struct Node* getNode(struct Table* tbl, uint32_t hash) {
    uint32_t pos = hash % tbl->na;
    uint32_t cmp = 0;
    struct Bucket* b;
    struct Node* res;

    if (hash == 0)
        return NULL;   

    b = tbl->a[pos];
    
    while (cmp != hash) {
        if (b == NULL)
            return NULL;
        cmp = b->hash;
        res = b->data;
        b = b->next;
    }

    return res;
}

void linkNodes(struct Node* p, struct Node* c, int cnt) {
    if (p->nc == p->cc) {
        p->nc *= 2;
        p->c = reallocarray(p->c, p->nc, sizeof(struct Node*));
        p->cn = reallocarray(p->cn, p->nc, sizeof(int));
    }
    p->c[p->cc] = c;
    p->cn[p->cc] = cnt;
    p->cc++;

    if (c->np == c->cp) {
        c->np *= 2;
        c->p = reallocarray(c->p, c->np, sizeof(struct Node*));
    }
    c->p[c->cp] = p;
    c->cp++;
}

void freeTable(struct Table* tbl) {
    struct Bucket *a, *b;
    
    for (int i = 0; i < tbl->na; i++) {
        b = tbl->a[i];

        while(b != NULL) {
            a = b;
            free(a->data->p);
            free(a->data->c);
            free(a->data->cn);
            free(a->data->str);
            free(a->data);
            b = a->next;
            free(a);
        }
    }

    free(tbl->a);
    free(tbl);
}

int countChildren(struct Node* node) {
    int res = 0;
    int cur;

    for (int i = 0; i < node->cc; i++) {
        cur = node->cn[i];
        res += cur;
        cur *= countChildren(node->c[i]);
        res += cur;
    }

    return res;
}

// Kindly taken from https://github.com/wolkykim/qlibc/blob/master/src/utilities/qhash.c
uint32_t qhashmurmur3_32(const void *data, size_t nbytes) {
    if (data == NULL || nbytes == 0)
        return 0;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const int nblocks = nbytes / 4;
    const uint32_t *blocks = (const uint32_t *) (data);
    const uint8_t *tail = (const uint8_t *) (data + (nblocks * 4));

    uint32_t h = 0;

    int i;
    uint32_t k;
    for (i = 0; i < nblocks; i++) {
        k = blocks[i];

        k *= c1;
        k = (k << 15) | (k >> (32 - 15));
        k *= c2;

        h ^= k;
        h = (h << 13) | (h >> (32 - 13));
        h = (h * 5) + 0xe6546b64;
    }

    k = 0;
    switch (nbytes & 3) {
        case 3:
            k ^= tail[2] << 16;
        case 2:
            k ^= tail[1] << 8;
        case 1:
            k ^= tail[0];
            k *= c1;
            k = (k << 15) | (k >> (32 - 15));
            k *= c2;
            h ^= k;
    };

    h ^= nbytes;

    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

int main(int argc, char** argv) {
    if (argc != 2)
        return -1;

    int lnsz = 128,
        sz = 20;
    int len = 0;
    int n = 0;
    char line[lnsz];
    char* buf;
    struct Node** nodes = calloc(sz, sizeof(struct Node*));
    FILE* fp = fopen(argv[1], "r");
     
    if (!fp)
        return -2;

    while (fgets(line, lnsz, fp) != NULL) {        
        len = strstr(line, "contain") - line;
        buf = calloc(len, sizeof(char));

        snprintf(buf, len, "%s", line);
        buf[len - 1] = '\0';
        nodes[n] = mkNode(buf, len);
        
        if (++n == sz) {
            sz *= 2;
            nodes = reallocarray(nodes, sz, sizeof(struct Node*));
        }
    }

    struct Table* tbl = calloc(1, sizeof(struct Table));
    tbl->na = 3*n;
    tbl->a = calloc(tbl->na, sizeof(struct Bucket*));
    for (int i = 0; i < tbl->na; i++) tbl->a[i] = NULL;
    uint32_t hash;

    for (int i = 0; i < n; i++) {
        hash = qhashmurmur3_32(nodes[i]->str, sizeof(char) * nodes[i]->sz);
        insert(tbl, nodes[i], hash);
    }
    rewind(fp);
    
    char* str;
    int offset;
    len = 40;
    char tok[len];
    int cnt;
    int m = 0;
    struct Node *node, *adj;
    
    while (fgets(line, lnsz, fp) != NULL) {        
        str = strstr(line, "contain") + 8;
        offset = 0;
        node = nodes[m];
        
        while (sscanf(str+offset, "%[^,.\n] ", tok) > 0) {
            offset += strlen(tok) + 2;
            if (sscanf(tok, "%d", &cnt) > 0) {
                len = strlen(tok + 2);
                buf = calloc(len + 2, sizeof(char));
                strncpy(buf, tok + 2, len);                
                if (buf[len - 1] != 's')
                    buf[len++] = 's'; 
                buf[len] = '\0';
                
                hash = qhashmurmur3_32(buf, sizeof(char) * (len + 1));
                adj = getNode(tbl, hash);
                free(buf);

                linkNodes(node, adj, cnt);
            }
        }

        m++;
    }
    
    free(nodes);
    fclose(fp);

    char* target = "shiny gold bags";
    node = getNode(tbl, qhashmurmur3_32(target, sizeof(char) * (strlen(target) + 1)));
    int res = countChildren(node);

    freeTable(tbl);
    printf("%d\n", res);
    
    return 0;
}

