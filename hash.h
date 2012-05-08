#ifndef _HASH_H
#define _HASH_H

#include <string.h>
#include <stddef.h>

/* An invalid integer->pointer cast can cause undefined behavior, and
   without uintptr_t or C99 long long, there is no way to avoid a
   potential compiler warning about casting to a shorter integer type.
   So try to use uintptr_t. */
#if defined(_MSC_VER)
typedef uintptr_t _h_ptr;
#elif __STDC_VERSION__ >= 199901L || defined(__GNUC__)
#include <stdint.h>
typedef uintptr_t _h_ptr;
#else
typedef void *_h_ptr;
#endif

#if defined(__GNUC__) || defined(_MSC_VER)
#define H_INLINE __inline
#elif __STDC_VERSION__ >= 199901L || __cplusplus
#define H_INLINE inline
#else
#define H_INLINE 
#endif

#define HASH_HEAD(head_struct) \
    struct head_struct { \
        size_t hh_count; \
        size_t hh_nbuckets; \
        _h_ptr *hh_buckets; \
    }

#define HASH_ENTRY(head_struct) \
    struct { \
        _h_ptr he_next; \
    }

#define HASH_PARAMS(head_struct, entry_struct, entry_field, hash_func, eq_func) \
    typedef struct entry_struct head_struct##__entry; \
    static H_INLINE head_struct##__entry * \
    head_struct##__seek(const struct head_struct *head, \
                        size_t idx) { \
        _h_ptr result; \
        for(; idx < head->hh_nbuckets; idx++) { \
            result = head->hh_buckets[idx]; \
            if(result) { \
                return (head_struct##__entry *) result; \
            } \
        } \
        return 0; \
    } \
    static H_INLINE head_struct##__entry * \
    head_struct##__next(const struct head_struct *head, \
                        const head_struct##__entry *entry) { \
        _h_ptr next = entry->entry_field.he_next; \
        return ((size_t) next & 1) ? \
            head_struct##__seek(head, (size_t) next >> 1) : \
            (head_struct##__entry *) next; \
    } \
    static H_INLINE head_struct##__entry * \
    head_struct##__lookup(struct head_struct *head, \
                          const head_struct##__entry *proto, \
                          char remove, \
                          head_struct##__entry *add) { \
        head_struct##__entry *entry; \
        _h_ptr next, *nextp; \
        size_t hash; \
        if(!head->hh_nbuckets) return 0; \
        hash = (hash_func)(proto) % head->hh_nbuckets; \
        nextp = &head->hh_buckets[hash]; \
        next = *nextp; \
        if(next == 0) next = (_h_ptr) ((hash << 1) + 3); \
        if(add) { \
            *nextp = (_h_ptr) add; \
            nextp = &add->entry_field.he_next; \
            *nextp = next; \
            head->hh_count++; \
        } \
        for(; \
            !((size_t) next & 1); \
            nextp = &entry->entry_field.he_next, \
            next = *nextp) { \
            unsigned char cmp; \
            entry = (head_struct##__entry *) next; \
            cmp = (eq_func)(entry, proto); \
            if(cmp) { \
                if(remove) { \
                    next = entry->entry_field.he_next; \
                    *nextp = ((size_t) next & 1) ? next : 0; \
                    head->hh_count--; \
                } \
                return entry; \
            } \
        } \
        return 0; \
    } \
    static H_INLINE head_struct##__entry * \
    head_struct##__set(struct head_struct *head, \
                       head_struct##__entry *entry) { \
        return head_struct##__lookup(head, entry, 1, entry); \
    } \
    static H_INLINE _h_ptr * \
    head_struct##__resize(struct head_struct *head, \
                          void *buf, \
                          size_t nbuckets) { \
        struct head_struct old = *head; \
        head_struct##__entry *entry, *t; \
        head->hh_nbuckets = nbuckets; \
        head->hh_buckets = (_h_ptr *) buf; \
        memset(buf, 0, HASH_ALLOC_SIZE(head, nbuckets)); \
        HASH_FOREACH_SAFE(entry, &old, head_struct, t) { \
            HASH_SET(head, entry, head_struct); \
        } \
        return old.hh_buckets; \
    } \
    static H_INLINE char \
    head_struct##__resize_alloc(struct head_struct *head, \
                                void *buf, \
                                void (*deallocator)(void *), \
                                size_t nbuckets) { \
        if(buf) { \
            deallocator(head_struct##__resize(head, buf, nbuckets)); \
            return 1; \
        } else { \
            return 0; \
        } \
    } \
    typedef int head_struct##__allow_final_semicolon

#define HASH_INIT(head) \
    /* initialize to an empty hash */ \
    do { \
        head.hh_count = 0; \
        head.hh_nbuckets = 0; \
        head.hh_buckets = 0; \
    } while(0)

#define HASH_HEAD_INITIALIZER { 0, 0, 0 }

#define HASH_ENTRY_INITIALIZER { 0 }

#define HASH_EMPTY(head) (!HASH_COUNT(head))

#define HASH_COUNT(head) ((head)->hh_count)

#define HASH_NBUCKETS(head) ((head)->hh_nbuckets)

#define HASH_FIRST(head, head_struct) \
    head_struct##__seek(head, 0)

#define HASH_NEXT(head, elm, head_struct) \
    head_struct##__next(head, elm)

#define HASH_FOREACH(var, head, head_struct) \
    for ((var) = HASH_FIRST(head, head_struct); \
         (var); \
         (var) = HASH_NEXT(head, var, head_struct))

#define HASH_FOREACH_SAFE(var, head, head_struct, tvar) \
    for ((var) = HASH_FIRST(head, head_struct); \
         (var) && ((tvar) = HASH_NEXT(head, var, head_struct), 1); \
         (var) = (tvar))

#define HASH_LOOKUP(head, proto, head_struct) \
    head_struct##__lookup(head, proto, 0, 0)

#define HASH_SET(head, entry, head_struct) \
    head_struct##__set(head, entry)

#define HASH_REMOVE(head, proto, head_struct) \
    head_struct##__lookup(head, proto, 1, 0)

#define HASH_RESIZE(head, buckets, nbuckets, head_struct) \
    head_struct##__resize(head, buckets, nbuckets)

#define HASH_ALLOC_SIZE(head, nbuckets) \
    (sizeof(_h_ptr) * (nbuckets))

#define HASH_RESIZE_ALLOC(head, allocator, deallocator, nbuckets, head_struct) \
    head_struct##__resize_alloc( \
        head, \
        (allocator)(HASH_ALLOC_SIZE(head, nbuckets)), \
        deallocator, \
        nbuckets)

#endif
