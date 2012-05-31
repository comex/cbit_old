#if CLOSED
#define XHASH CHASH
#else
#define XHASH HASH
#endif
#define _H__(a, b) b##_##a
#define _H_(a, b) _H__(a, b)
#define _H(a) _H_(a, XHASH)

/* verify that it works without any includes */
#include "hash.h"
static _H(HEAD)(s2s) a_hash = HASH_HEAD_INITIALIZER;
struct s2s_entry {
#if !CLOSED
    HASH_ENTRY(s2s) he;
#endif
    const char *key;
    const char *value;
};
static __inline char s2s_eq(const struct s2s_entry *, const struct s2s_entry *);
static __inline unsigned long s2s_hash(const struct s2s_entry *);
__attribute__((unused))
static __inline char s2s_null(const struct s2s_entry *);
#if CLOSED
CHASH_PARAMS(s2s, s2s_entry, s2s_hash, s2s_eq, s2s_null);
#else
HASH_PARAMS(s2s, s2s_entry, he, s2s_hash, s2s_eq);
#endif


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char s2s_eq(const struct s2s_entry *a, const struct s2s_entry *b) {
    return !strcmp(a->key, b->key);
}

unsigned long s2s_hash(const struct s2s_entry *a) {
    const char *s = a->key;
    unsigned char c;
    unsigned long ret = 2166136261;
    for(; (c = *s); s++) {
        ret = (ret ^ c) * 16777619;
    }
    return ret;
}

static char s2s_null(const struct s2s_entry *a) {
    return !a->key;
}

static void set(const char *k, const char *v) {
    struct s2s_entry *a = (struct s2s_entry *) malloc(sizeof(*a));
    a->key = k;
    a->value = v;
    _H(SET)(&a_hash, a, s2s);
}

static void dump() {
    struct s2s_entry *a;
    printf("{\n");
    _H(FOREACH)(a, &a_hash, s2s) {
        printf("%s=%s\n", a->key, a->value);
    }
    printf("}\n");
}

int main() {
    _H(RESIZE_ALLOC)(&a_hash, malloc, free, 10, s2s);
    set("mia", "fey");
    set("foo", "bar");
    set("foo", "derp");
    dump();
    _H(RESIZE_ALLOC)(&a_hash, malloc, free, HASH_SUGGESTED_SIZE(&a_hash, s2s), s2s);
    dump();
    return 0;        
}
