#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct num {
    char v;
    struct num *n;
} num;

num *alloc(char v, num *n) {
    num *r = malloc(sizeof(num));
    r->v = v;
    r->n = n;
    return r;
}

void kill(num *a) {
    while (a) {
        num *b = a->n;
        free(a);
        a = b;
    }
}

num *cache = NULL;

num *get(char v, num *n) {
    num *r;
    if (cache) {
        r = cache;
        cache = cache->n;
    } else {
        r = malloc(sizeof(num));
    }
    r->v = v;
    r->n = n;
    return r;
}

void put(num *a) {
    while (a) {
        num *b = a->n;
        a->n = cache;
        cache = a;
        a = b;
    }
}

num *fact(int N) {
    int i;
    num *a = alloc(1, NULL);
    for (i = 2 ; i <= N ; i++) {
        num *ai = a, *p = alloc(0, NULL), *pi = p, *pc;
        while (ai) {
            int c = ai->v * i;
            pc = pi;
            while (c > 0) {
                c += pc->v;
                pc->v = c % 10;
                c /= 10;
                if (!pc->n) pc->n = alloc(0, NULL);
                pc = pc->n;
            }
            ai = ai->n;
            if (!pi->n) pi->n = alloc(0, NULL); // This is necessary to handle situations where 'c' starts as zero
            pi = pi->n;
        }
        kill(a);
        a = p;
    }

    // Inverts the order of the digits
    num *ai = a, *r = NULL;
    while (ai) {
        r = alloc(ai->v, r);
        ai = ai->n;
    }
    kill(a);

    num *ri = r;
    // Remove leading zeroes
    while (ri && ri->v == 0) ri = ri->n;

    return ri;
}

num *fact_cache(int N) {
    int i;
    num *a = get(1, NULL);
    for (i = 2 ; i <= N ; i++) {
        num *ai = a, *p = get(0, NULL), *pi = p, *pc;
        while (ai) {
            int c = ai->v * i;
            pc = pi;
            while (c > 0) {
                c += pc->v;
                pc->v = c % 10;
                c /= 10;
                if (!pc->n) pc->n = get(0, NULL);
                pc = pc->n;
            }
            ai = ai->n;
            if (!pi->n) pi->n = get(0, NULL); // This is necessary to handle situations where 'c' starts as zero
            pi = pi->n;
        }
        put(a);
        a = p;
    }

    // Inverts the order of the digits
    num *ai = a, *r = NULL;
    while (ai) {
        r = get(ai->v, r);
        ai = ai->n;
    }
    put(a);

    num *ri = r;
    // Remove leading zeroes
    while (ri && ri->v == 0) ri = ri->n;

    return ri;
}

int main()
{
    num *r = fact(500);

    while (r) {
        printf("%d", r->v);
        r = r->n;
    }
    printf("\n");
    kill(r);

    clock_t start, stop;
    start = clock();
    int i;
    for (i = 0 ; i < 100 ; i++) {
        r = fact(500);
        kill(r);
    }
    stop = clock();
    printf("Took an average of %0.7f seconds\n", (double)(stop - start)/CLOCKS_PER_SEC/100);

    start = clock();
    for (i = 0 ; i < 100 ; i++) {
        r = fact_cache(500);
        put(r);
    }
    kill(cache);
    stop = clock();
    printf("Took an average of %0.7f seconds\n", (double)(stop - start)/CLOCKS_PER_SEC/100);
    return 0;
}
