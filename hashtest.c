/* verify that it works without any includes */
#include "hash.h"
static HASH_HEAD(s2s) a_hash = HASH_HEAD_INITIALIZER;
struct s2s_entry {
    HASH_ENTRY(s2s) he;
    const char *key;
    const char *value;
};
static char s2s_eq(const struct s2s_entry *, const struct s2s_entry *);
static unsigned long s2s_hash(const struct s2s_entry *);
HASH_PARAMS(s2s, s2s_entry, he, s2s_hash, s2s_eq);

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

static void set(const char *k, const char *v) {
    struct s2s_entry *a = (struct s2s_entry *) malloc(sizeof(*a));
    a->key = k;
    a->value = v;
    HASH_SET(&a_hash, a, s2s);
}

static void dump() {
    struct s2s_entry *a;
    printf("{\n");
    HASH_FOREACH(a, &a_hash, s2s) {
        printf("%s=%s\n", a->key, a->value);
    }
    printf("}\n");
}

int main() {
    HASH_RESIZE_ALLOC(&a_hash, malloc, free, 10, s2s);
    set("mia", "fey");
    set("foo", "bar");
    set("foo", "derp");
    dump();
    HASH_RESIZE_ALLOC(&a_hash, malloc, free, 20, s2s);
    dump();
    return 0;        
}
