#include <stdio.h>
#include <stdlib.h>

typedef struct num {
    char v;
    struct num *n;
} num;

num *get(char v, num *n) {
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

num *fact(int N) {
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
        kill(a);
        a = p;
    }

    // Inverts the order of the digits
    num *ai = a, *r = NULL;
    while (ai) {
        r = get(ai->v, r);
        ai = ai->n;
    }
    kill(a);

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
    kill(r);
    return 0;
}
