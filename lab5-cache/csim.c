#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "cachelab.h"

typedef struct ListNode {
    unsigned tag, valid;
    struct ListNode* next;
} ListNode;

typedef struct Set {
    unsigned E;
    ListNode* line_head;
    ListNode* tail;
} Set;

typedef struct Cache {
    unsigned S;
    Set* sets;
} Cache;

ListNode* createNode(int valid, int tag) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->valid = valid;
    newNode->tag = tag;
    newNode->next = NULL;
    return newNode;
}

// create a List with length of E+1
ListNode* createList(unsigned E) {
    ListNode* head = createNode(0, 0);
    ListNode* p = head;
    for (unsigned i = 0; i < E; i++) {
        ListNode* newNode = createNode(0, 0);
        p->next = newNode;
        p = p->next;
    }
    return head;
}

Cache* buildCache(unsigned s, unsigned E) {
    /*
    s: Number of set index bits (S = 2^s is the number of sets)
    E: Associativity (number of lines per set)
    b: Number of block bits (B = 2^b is the block size)
    */
    unsigned S = (1 << s);
    Cache* c = (Cache*)malloc(sizeof(Cache));
    c->S = S;
    c->sets = (Set*) malloc(S * sizeof(Set));
    for (int i = 0; i < S; i++) {
        c->sets[i].E = E;
        c->sets[i].line_head = createList(E);
        c->sets[i].tail = c->sets[i].line_head;
    }
    return c;
}

unsigned setIndex(unsigned long addr, int s, int b) {
    unsigned mask = (1 << s) - 1;
    return mask & (addr >> b);
}

unsigned getTag(unsigned long addr, int s, int b) {
    return addr >> (s + b);
}

int accessMem(Cache* cache, unsigned iset, unsigned tag) {
    Set* set = cache->sets + iset;
    ListNode* p = set->line_head->next;
    // hit
    while (p != NULL && p != set->tail->next) {
        if (p->valid && p->tag == tag)
            return 0;
        p = p->next;
    }
    // cold miss
    if (set->tail->next) {
        set->tail = set->tail->next;
        set->tail->valid = 1;
        set->tail->tag = tag;
        return 1;
    }
    // conflict miss with eviction
    set->line_head = set->line_head->next;  // evict
    set->tail->next = createNode(1, tag);
    set->tail = set->tail->next;
    return 2;
}

int main(int argc, char *argv[])
{
    unsigned s_value = 0, E_value = 0, b_value = 0;
    int verbose = 0;
    char *t_value = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "s:E:b:t:v"))!= -1) {
        switch (opt) {
            case 's':
                s_value = atoi(optarg);
                break;
            case 'E':
                E_value = atoi(optarg);
                break;
            case 'b':
                b_value = atoi(optarg);
                break;
            case 't':
                t_value = optarg;
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s -s num -E num -b num -t file_path\n", argv[0]);
                return 1;
        }
    }
    Cache *cache = buildCache(s_value, E_value);
    char line[1024];
    FILE *file = fopen(t_value, "r");
    int hit = 0, miss = 0, evict = 0;
    while (fgets(line, sizeof(line), file)) {
        unsigned long address;
        char operation;
        int size;
        if (sscanf(line, " %c %lx,%d", &operation, &address, &size) == 3) {
            if (operation == 'I') continue;
            unsigned iset = setIndex(address, s_value, b_value);
            unsigned tag = getTag(address, s_value, b_value);
            int code = accessMem(cache, iset, tag);
            if (verbose)
                printf("%c %lx,%d", operation, address, size);
            switch (code) {
            case 0: // hit
                hit++;
                if (verbose) printf(" hit");
                break;
            case 1:
                // cold miss
                miss++;
                if (verbose) printf(" miss");
                break;
            case 2:
                // conflict miss
                miss++;
                evict++;
                if (verbose) printf(" miss eviction");
                break;
            default:
                break;
            }
            if (operation == 'M') {
                hit++;
                if (verbose) printf(" hit");
            }
            if (verbose) printf("\n");
        } else {
            printf("Error:%s\n", line);
        }
    }
    printSummary(hit, miss, evict);
    return 0;
}
