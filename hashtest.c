#if CLOSED
#define XHASH CHASH
#else
#define XHASH HASH
#endif
#if EXT
#define XEXT(macro, params, less) macro##_EXTREF less; macro##_EXTDEF params;
#else
#define XEXT(macro, params, less) macro params;
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
static __inline char s_eq(const char *a, const char *b);
static __inline size_t s_hash(const char *a);
static __inline char s2s_eq(const struct s2s_entry *, const struct s2s_entry *);
static __inline size_t s2s_hash(const struct s2s_entry *);
__attribute__((unused))
static __inline char s2s_null(const struct s2s_entry *);
#if CLOSED
XEXT(CHASH_PARAMS, (s2s, s2s_entry, s2s_hash, s2s_eq, s2s_null), (s2s, s2s_entry))
#else
XEXT(HASH_PARAMS, (s2s, s2s_entry, he, s2s_hash, s2s_eq), (s2s, s2s_entry))
#endif

/* need malloc for this */
#include <stdlib.h>

XEXT(NHASH_PARAMS, (sdict, sdict_entry, const char *, const char *, s_hash, s_eq), (sdict, sdict_entry, const char *, const char *))

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char s_eq(const char *a, const char *b) {
    return !strcmp(a, b);
}

size_t s_hash(const char *s) {
    unsigned char c;
    size_t ret = 2166136261;
    for(; (c = *s); s++) {
        ret = (ret ^ c) * 16777619;
    }
    return ret;
}

char s2s_eq(const struct s2s_entry *a, const struct s2s_entry *b) {
    return s_eq(a->key, b->key);
}

size_t s2s_hash(const struct s2s_entry *a) {
    return s_hash(a->key);
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

static void dump_one() {
    struct s2s_entry *a;
    printf("{\n");
    _H(FOREACH)(a, &a_hash, s2s) {
        printf("%s=%s\n", a->key, a->value);
    }
    printf("}\n");
}

static void dump() {
    dump_one();
#if __STDC_VERSION__ >= 199901L && !CLOSED
    printf("{\n");
    HASH_FOREACH_EXT(a, &a_hash, s2s) {
        printf("%s=%s\n", a->key, a->value);
    }
    HASH_FOREACH_EXT(a, &a_hash, s2s) {
        (void) a;
    } // verify duplicates work
    printf("}\n");
#else
    dump_one();
#endif
}

static void sdump_one(struct sdict *sd) {
    printf("n{\n");
    struct sdict_entry *entry;
    const char *k, *v;
    NHASH_FOREACH(k, v, sd, entry, sdict) {
        printf("%s=%s\n", k, v);
    }
    printf("} %zd\n", NHASH_COUNT(sd));
}

static void sdump(struct sdict *sd) {
    sdump_one(sd);
#if __STDC_VERSION__ >= 199901L
    int count = 0;
    printf("n{\n");
    NHASH_FOREACH_EXT(k, v, sd, sdict) {
        printf("%s=%s\n", k, v);
        count++;
        continue;
    }
    NHASH_FOREACH_EXT(k, v, sd, sdict) {
        (void) k;
        (void) v;
        count++;
        break;
    }
    printf("} %d\n", count - 1);
#else
    sdump_one(sd);
#endif

}

int main() {
    _H(RESIZE_ALLOC)(&a_hash, malloc, free, 10, s2s);
    set("mia", "fey");
    set("foo", "bar");
    set("foo", "derp");
    dump();
    _H(RESIZE_ALLOC)(&a_hash, malloc, free, HASH_SUGGESTED_SIZE(&a_hash, s2s), s2s);
    dump();
    struct s2s_entry proto;
    proto.key = "foo";
    _H(REMOVE)(&a_hash, &proto, s2s);
    dump();

    struct sdict *sd = NHASH_ALLOC(sdict);
    NHASH_SET(sd, "mia", "fey", sdict);
    NHASH_SET(sd, "foo", "bar", sdict);
    NHASH_SET(sd, "foo", "derp", sdict);
    sdump(sd);
    NHASH_REMOVE(sd, "foo", sdict);
    sdump(sd);

    return 0;
}
