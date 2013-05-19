#ifndef _CBIT_HASH_H
#define _CBIT_HASH_H

#include <string.h>
#include <stddef.h>

/* An invalid integer->pointer cast can cause undefined behavior, and
   without uintptr_t or C99 long long, there is no way to avoid a
   potential compiler warning about casting to a shorter integer type.
   So try to use uintptr_t. */
#if HASH_FORCE_VP_HASH_PTR
typedef void *_hash_ptr;
#else
#if defined(_MSC_VER)
typedef uintptr_t _hash_ptr;
#elif __STDC_VERSION__ >= 199901L || defined(__GNUC__)
#include <stdint.h>
typedef uintptr_t _hash_ptr;
#else
typedef void *_hash_ptr;
#endif
#endif

#if defined(__GNUC__) || defined(_MSC_VER)
#define _HASH_INLINE static __inline
#elif __STDC_VERSION__ >= 199901L || __cplusplus
#define _HASH_INLINE static inline
#else
#define _HASH_INLINE static
#endif

#define _HASH_PASTE2(x, y) x##__##y
#define _HASH_PASTE(x, y) _HASH_PASTE2(x, y)
#define _HASH_LINE(x) _HASH_PASTE(x, __LINE__)

#define _HASH_COMMON_BITS(head_struct, alloc_size, foreach_safe, t_ty, set, \
                          func_decl) \
    func_decl size_t \
    head_struct##__hash_suggested_size(const struct head_struct *head) { \
        if (head->hh_count * 4 < head->hh_nbuckets) { \
            size_t result = head->hh_count * 2 + 1; \
            if (result < 5) result = 5; \
            return result; \
        } else if (head->hh_count * 3 / 2 >= head->hh_nbuckets) { \
            return head->hh_nbuckets * 2 + 1; \
        } else { \
            return head->hh_nbuckets; \
        } \
    } \
    func_decl void * \
    head_struct##__hash_resize(struct head_struct *head, \
                               void *buf, \
                               size_t nbuckets) { \
        struct head_struct old = *head; \
        head_struct##__hash_entry *entry; \
        t_ty t; \
        head->hh_nbuckets = nbuckets; \
        head->hh_buckets = (_hash_ptr *) buf; \
        head->hh_count = 0; \
        memset(buf, 0, alloc_size(nbuckets, head_struct)); \
        if (old.hh_nbuckets) { \
            foreach_safe(entry, &old, head_struct, t) { \
                set(head, entry, head_struct); \
            } \
        } \
        return (_hash_ptr *) old.hh_buckets; \
    } \
    func_decl char \
    head_struct##__hash_resize_alloc(struct head_struct *head, \
                                     void *(*allocator)(size_t), \
                                     void (*deallocator)(void *), \
                                     size_t nbuckets) { \
        void *buf; \
        if (nbuckets == head->hh_nbuckets) \
            return 1; \
        buf = allocator(alloc_size(nbuckets, head_struct)); \
        if (buf) { \
            deallocator(head_struct##__hash_resize(head, buf, nbuckets)); \
            return 1; \
        } else { \
            return 0; \
        } \
    } \
    typedef int head_struct##__hash_allow_final_semicolon

#define _HASH_COMMON_BITS_EXTREF(head_struct) \
    extern size_t \
    head_struct##__hash_suggested_size(const struct head_struct *head); \
    extern void * \
    head_struct##__hash_resize(struct head_struct *head, \
                               void *buf, \
                               size_t nbuckets); \
    extern char \
    head_struct##__hash_resize_alloc(struct head_struct *head, \
                                     void *(*allocator)(size_t), \
                                     void (*deallocator)(void *), \
                                     size_t nbuckets)

#define HASH_HEAD(head_struct) \
    struct head_struct { \
        size_t hh_count; \
        size_t hh_nbuckets; \
        _hash_ptr *hh_buckets; \
    }

#define HASH_ENTRY(head_struct) \
    struct { \
        _hash_ptr he_next; \
    }

/* Note that the callbacks supplied here are called without parentheses, so
   they can be macros, but extra parentheses are required if the callback is,
   e.g., a conditional expression. */

#define HASH_PARAMS(head_struct, entry_struct, entry_field, \
                    hash_func, eq_func) \
    typedef struct entry_struct head_struct##__hash_entry; \
    _HASH_PARAMS(head_struct, entry_struct, entry_field, \
                 hash_func, eq_func, \
                 _HASH_INLINE)

#define HASH_PARAMS_EXTDEF(head_struct, entry_struct, entry_field, \
                           hash_func, eq_func) \
    _HASH_PARAMS(head_struct, entry_struct, entry_field, \
                 hash_func, eq_func, \
                 extern)

#define _HASH_PARAMS(head_struct, entry_struct, entry_field, \
                     hash_func, eq_func, \
                     func_decl) \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_seek(const struct head_struct *head, \
                             size_t idx) { \
        _hash_ptr result; \
        for (; idx < HASH_NBUCKETS(head); idx++) { \
            result = head->hh_buckets[idx]; \
            if (result && !((size_t) result & 1)) \
                return (head_struct##__hash_entry *) result; \
        } \
        return NULL; \
    } \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_next(const struct head_struct *head, \
                             const head_struct##__hash_entry *entry) { \
        _hash_ptr next = entry->entry_field.he_next; \
        return ((size_t) next & 1) ? \
            head_struct##__hash_seek(head, (size_t) next >> 1) : \
            (head_struct##__hash_entry *) next; \
    } \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_lookup(const struct head_struct *_head, \
                               const head_struct##__hash_entry *proto, \
                               char remove, \
                               char add) { \
        struct head_struct *head = (struct head_struct *) _head; \
        head_struct##__hash_entry *entry; \
        _hash_ptr next, *nextp; \
        size_t hash; \
        if (!HASH_NBUCKETS(head)) return NULL; \
        hash = hash_func(proto) % HASH_NBUCKETS(head); \
        nextp = &head->hh_buckets[hash]; \
        next = *nextp; \
        if (next == 0) \
            next = (_hash_ptr) ((hash << 1) + 3); \
        if (add) { \
            *nextp = (_hash_ptr) proto; \
            nextp = (_hash_ptr *) &proto->entry_field.he_next; \
            *nextp = next; \
            head->hh_count++; \
        } \
        for (; \
             !((size_t) next & 1); \
             nextp = &entry->entry_field.he_next, \
             next = *nextp) { \
            unsigned char cmp; \
            entry = (head_struct##__hash_entry *) next; \
            cmp = eq_func(entry, proto); \
            if (cmp) { \
                if (remove) { \
                    next = entry->entry_field.he_next; \
                    *nextp = next; \
                    head->hh_count--; \
                } \
                return entry; \
            } \
        } \
        return NULL; \
    } \
    _HASH_COMMON_BITS(head_struct, \
                      HASH_ALLOC_SIZE, \
                      HASH_FOREACH_SAFE, \
                      head_struct##__hash_entry *, \
                      HASH_SET, \
                      func_decl) \

#define HASH_PARAMS_EXTREF(head_struct, entry_struct) \
    typedef struct entry_struct head_struct##__hash_entry; \
    extern head_struct##__hash_entry * \
    head_struct##__hash_seek(const struct head_struct *head, \
                             size_t idx); \
    extern head_struct##__hash_entry * \
    head_struct##__hash_next(const struct head_struct *head, \
                             const head_struct##__hash_entry *entry); \
    extern head_struct##__hash_entry * \
    head_struct##__hash_lookup(const struct head_struct *head, \
                               const head_struct##__hash_entry *proto, \
                               char remove, \
                               char add); \
    _HASH_COMMON_BITS_EXTREF(head_struct)

#define HASH_INIT(head) \
    /* initialize to an empty hash */ \
    do { \
        head->hh_count = 0; \
        head->hh_nbuckets = 0; \
        head->hh_buckets = NULL; \
    } while (0)

#define HASH_HEAD_INITIALIZER { 0, 0, NULL }

#define HASH_ENTRY_INITIALIZER { 0 }

#define HASH_EMPTY(head) (!HASH_COUNT(head))

#define HASH_COUNT(head) ((head)->hh_count)

#define HASH_NBUCKETS(head) ((head)->hh_nbuckets)

#define HASH_FIRST(head, head_struct) \
    head_struct##__hash_seek(head, 0)

#define HASH_NEXT(head, elm, head_struct) \
    head_struct##__hash_next(head, elm)

#define HASH_FOREACH(var, head, head_struct) \
    for ((var) = HASH_FIRST(head, head_struct); \
         (var); \
         (var) = HASH_NEXT(head, var, head_struct))

#define HASH_FOREACH_SAFE(var, head, head_struct, tvar) \
    for ((var) = HASH_FIRST(head, head_struct); \
         (var) && ((tvar) = HASH_NEXT(head, var, head_struct), 1); \
         (var) = (tvar))

/* Like HASH_FOREACH_SAFE, but without the separate declaration and requires
   C99. */

#define HASH_FOREACH_EXT(var, head, head_struct) \
    for (head_struct##__hash_entry *_hash_t, \
            *var = HASH_FIRST(head, head_struct); \
         (var) && ((_hash_t) = HASH_NEXT(head, var, head_struct), 1); \
         (var) = (_hash_t))

#define HASH_LOOKUP(head, proto, head_struct) \
    head_struct##__hash_lookup(head, proto, 0, 0)

#define HASH_SET(head, entry, head_struct) \
    head_struct##__hash_lookup(head, entry, 1, 1)

#define HASH_REMOVE(head, proto, head_struct) \
    head_struct##__hash_lookup(head, proto, 1, 0)

#define HASH_ALLOC_SIZE(nbuckets, head_struct) \
    (sizeof(_hash_ptr) * (nbuckets))

#define HASH_RESIZE(head, buckets, nbuckets, head_struct) \
    head_struct##__hash_resize(head, buckets, nbuckets)

#define HASH_RESIZE_ALLOC(head, allocator, deallocator, nbuckets, head_struct) \
    head_struct##__hash_resize_alloc(head, allocator, deallocator, nbuckets)

#define HASH_SUGGESTED_SIZE(head, head_struct) \
    head_struct##__hash_suggested_size(head)

/* Closed hashes */

#define CHASH_HEAD(head_struct) \
    struct head_struct { \
        size_t hh_count; \
        size_t hh_nbuckets; \
        void *hh_buckets; \
    }

#define CHASH_BUCKETS(head, head_struct) \
    ((head_struct##__hash_entry *) ((head)->hh_buckets))

/* null_func: Used to determine whether an entry is null/invalid and thus
   represents the end of a search.  Unused buckets are zeroed, so an all-zero
   value must be considered invalid, but typically it's possible to determine
   whether an entry is invalid by checking a single field rather than comparing
   the whole thing with zero. */

#define CHASH_PARAMS(head_struct, entry_struct, \
                     hash_func, eq_func, null_func) \
    typedef struct entry_struct head_struct##__hash_entry; \
    _CHASH_PARAMS(head_struct, entry_struct, \
                  hash_func, eq_func, null_func, \
                  _HASH_INLINE)

#define CHASH_PARAMS_EXTDEF(head_struct, entry_struct, \
                            hash_func, eq_func, null_func) \
    _CHASH_PARAMS(head_struct, entry_struct, \
                  hash_func, eq_func, null_func, \
                  extern)

#define _CHASH_PARAMS(head_struct, entry_struct, \
                      hash_func, eq_func, null_func, \
                      func_decl) \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_lookup(const struct head_struct *_head, \
                               const head_struct##__hash_entry *proto, \
                               char mode) { \
        struct head_struct *head = (struct head_struct *) _head; \
        size_t hash, i, nbuckets = CHASH_NBUCKETS(head); \
        head_struct##__hash_entry \
            *bucket, *oldbucket, \
            *buckets = (head_struct##__hash_entry *) head->hh_buckets; \
        if (!nbuckets) \
            return NULL; \
        hash = i = hash_func(proto) % nbuckets; \
        do { \
            bucket = &buckets[i]; \
            if (null_func(bucket)) { \
                if (mode == 1 /* add */) { \
                    *bucket = *proto; \
                    head->hh_count++; \
                    return bucket; \
                } \
                return NULL; \
            } \
            if (eq_func(bucket, proto)) { \
                if (mode == 1 /* add */) \
                    *bucket = *proto; \
                else if (mode == 2 /* remove */) \
                    goto copy_backward; \
                return bucket; \
            } \
        } while ((i = (i + 1) % nbuckets) != hash); \
        return NULL; \
    copy_backward: \
        oldbucket = bucket; \
        while ((i = (i + 1) % nbuckets) != hash && \
               !null_func(bucket = &buckets[i]) && \
               hash_func(bucket) == hash) { \
            *oldbucket = *bucket; \
            oldbucket = bucket; \
        } \
        memset(oldbucket, 0, sizeof(*oldbucket)); \
        head->hh_count--; \
        return NULL; \
    } \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_next_by_idx(const struct head_struct *head, \
                                    size_t idx) { \
        size_t nbuckets = (head)->hh_nbuckets; \
        head_struct##__hash_entry *bucket = \
            &((head_struct##__hash_entry *) head->hh_buckets)[idx]; \
        for (; idx < nbuckets; idx++, bucket++) { \
            if (!null_func(bucket)) \
                return bucket; \
        } \
        return NULL; \
    } \
    _HASH_INLINE head_struct##__hash_entry * \
    head_struct##__hash_next(const struct head_struct *head, \
                        const head_struct##__hash_entry *elm) { \
        return head_struct##__hash_next_by_idx(head,  \
            elm - (head_struct##__hash_entry *) head->hh_buckets + 1); \
    } \
    _HASH_COMMON_BITS(head_struct, \
                      CHASH_ALLOC_SIZE, \
                      CHASH_FOREACH_SAFE, \
                      size_t, \
                      CHASH_SET, \
                      func_decl)

#define CHASH_PARAMS_EXTREF(head_struct, entry_struct) \
    typedef struct entry_struct head_struct##__hash_entry; \
    extern struct entry_struct * \
    head_struct##__hash_lookup(const struct head_struct *head, \
                               const head_struct##__hash_entry *proto, \
                               char mode); \
    extern struct entry_struct * \
    head_struct##__hash_next_by_idx(const struct head_struct *head, \
                                    size_t id); \
    _HASH_COMMON_BITS_EXTREF(head_struct)

#define CHASH_HEAD_INITIALIZER HASH_HEAD_INITIALIZER

#define CHASH_INIT HASH_INIT

#define CHASH_EMPTY HASH_EMPTY

#define CHASH_COUNT HASH_COUNT

#define CHASH_NBUCKETS HASH_NBUCKETS

#define CHASH_FIRST(head, head_struct) \
    head_struct##__hash_next_by_idx(head, 0)

#define CHASH_NEXT(head, elm, head_struct) \
    head_struct##__hash_next(head, elm)

#define CHASH_FOREACH(var, head, head_struct) \
    for ((var) = CHASH_FIRST(head, head_struct); \
         (var); \
         (var) = CHASH_NEXT(head, var, head_struct))

/* You must manually decrement idxvar after (successfully) removing
   an entry. */
#define CHASH_FOREACH_SAFE(var, head, head_struct, idxvar) \
    for ((idxvar) = 0; \
         (var) = head_struct##__hash_next_by_idx(head, idxvar), \
         (var) != NULL && \
         ((idxvar) = (var) - \
                     (head_struct##__hash_entry *) (head)->hh_buckets, 1); \
         (idxvar)++)

#define CHASH_LOOKUP(head, proto, head_struct) \
    head_struct##__hash_lookup(head, proto, 0)

#define CHASH_SET(head, entry, head_struct) \
    head_struct##__hash_lookup(head, entry, 1)

#define CHASH_REMOVE(head, proto, head_struct) \
    head_struct##__hash_lookup(head, proto, 2)

#define CHASH_ALLOC_SIZE(nbuckets, head_struct) \
    (sizeof(head_struct##__hash_entry) * (nbuckets))

#define CHASH_RESIZE HASH_RESIZE

#define CHASH_RESIZE_ALLOC HASH_RESIZE_ALLOC

#define CHASH_SUGGESTED_SIZE HASH_SUGGESTED_SIZE

/* "Nice" hash with auto-allocation of entries, auto-resizing of the table,
   and key-value.  Not appropriate for all use cases. */

#define NHASH_PARAMS(head_struct, entry_struct, key_ty, value_ty, \
                     hash_func, eq_func) \
    _NHASH_DEFINE_STUFF(head_struct, entry_struct, key_ty, value_ty); \
    _NHASH_PARAMS(head_struct, entry_struct, key_ty, value_ty, \
                  hash_func, eq_func, _HASH_INLINE)

#define NHASH_PARAMS_EXTDEF(head_struct, entry_struct, key_ty, value_ty, \
                            hash_func, eq_func) \
    _NHASH_PARAMS(head_struct, entry_struct, key_ty, value_ty, \
                  hash_func, eq_func, extern)

#define _NHASH_PARAMS(head_struct, entry_struct, key_ty, value_ty, \
                      hash_func, eq_func, \
                      func_decl) \
    _HASH_INLINE char \
    head_struct##__nhash_null(const struct entry_struct *a) { \
        return !a->es_key; \
    } \
    _HASH_INLINE char \
    head_struct##__nhash_eq(const struct entry_struct *a, \
                            const struct entry_struct *b) { \
        return eq_func(a->es_key, b->es_key); \
    } \
    _HASH_INLINE size_t \
    head_struct##__nhash_hash(const struct entry_struct *a) { \
        return hash_func(a->es_key); \
    } \
    CHASH_PARAMS(head_struct, entry_struct, \
                 head_struct##__nhash_hash, head_struct##__nhash_eq, \
                 head_struct##__nhash_null); \
    func_decl struct head_struct * \
    head_struct##__nhash_alloc(void) { \
        struct head_struct *head = \
            (struct head_struct *) malloc(sizeof(*head)); \
        CHASH_INIT(head); \
        return head; \
    } \
    func_decl value_ty \
    head_struct##__nhash_get(const struct head_struct *head, \
                             key_ty key) { \
        struct entry_struct proto, *entry; \
        proto.es_key = key; \
        entry = CHASH_LOOKUP(head, &proto, head_struct); \
        return entry ? entry->es_value : 0; \
    } \
    func_decl void \
    head_struct##__nhash_set(struct head_struct *head, \
                             key_ty key, \
                             value_ty value) { \
        struct entry_struct entry; \
        entry.es_key = key; \
        entry.es_value = value; \
        if(!CHASH_RESIZE_ALLOC(head, malloc, free, \
                               CHASH_SUGGESTED_SIZE(head, head_struct), \
                               head_struct)) { \
            abort(); \
        } \
        CHASH_SET(head, &entry, head_struct); \
    } \
    func_decl void \
    head_struct##__nhash_remove(struct head_struct *head, \
                                key_ty key) { \
        struct entry_struct proto; \
        proto.es_key = key; \
        CHASH_REMOVE(head, &proto, head_struct); \
    } \
    func_decl head_struct##__nhash_entry * \
    head_struct##__nhash_first(const struct head_struct *head) { \
        return CHASH_FIRST(head, head_struct); \
    } \
    func_decl head_struct##__nhash_entry * \
    head_struct##__nhash_next(const struct head_struct *head, \
                              const struct entry_struct *elm) { \
        return CHASH_NEXT(head, elm, head_struct); \
    } \
    typedef int head_struct##__nhash_allow_final_semicolon

#define NHASH_PARAMS_EXTREF(head_struct, entry_struct, key_ty, value_ty) \
    _NHASH_DEFINE_STUFF(head_struct, entry_struct, key_ty, value_ty); \
    extern struct head_struct * \
    head_struct##__nhash_alloc(void); \
    extern value_ty \
    head_struct##__nhash_get(const struct head_struct *head, \
                             key_ty key); \
    extern void \
    head_struct##__nhash_set(struct head_struct *head, \
                             key_ty key, \
                             value_ty value); \
    extern void \
    head_struct##__nhash_remove(struct head_struct *head, \
                                key_ty key); \
    extern head_struct##__nhash_entry * \
    head_struct##__nhash_first(const struct head_struct *head); \
    extern head_struct##__nhash_entry * \
    head_struct##__nhash_next(const struct head_struct *head, \
                              const struct entry_struct *elm) \

#define _NHASH_DEFINE_STUFF(head_struct, entry_struct, key_ty, value_ty) \
    CHASH_HEAD(head_struct); \
    struct entry_struct { \
        key_ty es_key; \
        value_ty es_value; \
    }; \
    typedef key_ty head_struct##__nhash_key; \
    typedef value_ty head_struct##__nhash_value; \
    typedef struct entry_struct head_struct##__nhash_entry \

#define NHASH_KEY(entry) (entry->es_key)

#define NHASH_VALUE(entry) (entry->es_value)

#define NHASH_ALLOC(head_struct) \
    head_struct##__nhash_alloc()

/* cheating */

#define NHASH_FREE(head, head_struct) \
    free(head)

#define NHASH_EMPTY HASH_EMPTY

#define NHASH_COUNT HASH_COUNT

#define NHASH_GET(head, key, head_struct) \
    head_struct##__nhash_get(head, key)

#define NHASH_SET(head, key, value, head_struct) \
    head_struct##__nhash_set(head, key, value)

#define NHASH_REMOVE(head, key, head_struct) \
    head_struct##__nhash_remove(head, key)

#define NHASH_FIRST(head, head_struct) \
    head_struct##__nhash_first(head)

#define NHASH_NEXT(head, elm, head_struct) \
    head_struct##__nhash_next(head, elm)

#define NHASH_FOREACH(key, value, head, entry, head_struct) \
    _NHASH_FOREACH(key, value, head, entry, head_struct, (entry)) \

#define _NHASH_FOREACH(key, value, head, entry, head_struct, entry_decl) \
    for (entry_decl = NHASH_FIRST(head, head_struct); \
         (entry) && (key = NHASH_KEY(entry), \
                   value = NHASH_VALUE(entry), \
                   1); \
         (entry) = NHASH_NEXT(head, (entry), head_struct))

/* Supports break and continue properly. */

#define NHASH_FOREACH_EXT(key, value, head, head_struct) \
    if (0) \
        _HASH_LINE(__nhash_finished): ; \
    else if (1) \
        goto _HASH_LINE(__nhash_body); \
    else \
        for (head_struct##__nhash_key key;;) \
            for (head_struct##__nhash_value value;;) \
                if (1) \
                    goto _HASH_LINE(__nhash_finished); \
                else \
                    _HASH_LINE(__nhash_body): \
                    _NHASH_FOREACH(key, value, head, _nhash_entry, \
                                   head_struct, \
                                   head_struct##__nhash_entry *_nhash_entry)

#endif
