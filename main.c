#include <stdio.h>
#include <stdlib.h>

struct num {
    char v;
    struct num *n;
};

struct num *get(char v, struct num *n) {
    struct num *r = malloc(sizeof(struct num));
    r->v = v;
    r->n = n;
    return r;
}

void kill(struct num *a) {
    while (a) {
        struct num *b = a->n;
        free(a);
        a = b;
    }
}

int main()
{
    int N = 500, i;
    struct num *a = get(1, NULL);
    for (i = 2 ; i <= N ; i++) {
        struct num *ai = a, *p = get(0, NULL), *pi = p, *pc;
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
    struct num *ai = a, *r = NULL;
    while (ai) {
        r = get(ai->v, r);
        ai = ai->n;
    }
    kill(a);

    struct num *ri = r;
    // Remove leading zeroes
    while (ri && ri->v == 0) ri = ri->n;
    // Print each digit
    while (ri) {
        printf("%d", ri->v);
        ri = ri->n;
    }
    kill(r);

    return 0;
}
