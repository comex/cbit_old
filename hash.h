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
#define _HASH_INLINE __inline
#elif __STDC_VERSION__ >= 199901L || __cplusplus
#define _HASH_INLINE inline
#else
#define _HASH_INLINE 
#endif

#define _HASH_COMMON_BITS(head_struct, alloc_size, foreach_safe, t_ty, set, \
                          func_decl) \
    func_decl size_t \
    head_struct##__hash_suggested_size(const struct head_struct *head) { \
        if(head->hh_count * 4 < head->hh_nbuckets) { \
            size_t result = head->hh_count * 2 + 1; \
            if(result < 5) result = 5; \
            return result; \
        } else if(head->hh_count * 3 / 2 >= head->hh_nbuckets) { \
            return head->hh_nbuckets * 2 + 1; \
        } else { \
            return head->hh_nbuckets; \
        } \
    } \
    func_decl _h_ptr * \
    head_struct##__hash_resize(struct head_struct *head, \
                               void *buf, \
                               size_t nbuckets) { \
        struct head_struct old = *head; \
        head_struct##__hash_entry *entry; \
        t_ty t; \
        head->hh_nbuckets = nbuckets; \
        head->hh_buckets = (_h_ptr *) buf; \
        memset(buf, 0, alloc_size(nbuckets, head_struct)); \
        if(old.hh_nbuckets) { \
            foreach_safe(entry, &old, head_struct, t) { \
                set(head, entry, head_struct); \
            } \
        } \
        return (_h_ptr *) old.hh_buckets; \
    } \
    func_decl char \
    head_struct##__hash_resize_alloc(struct head_struct *head, \
                                     void *(*allocator)(size_t), \
                                     void (*deallocator)(void *), \
                                     size_t nbuckets) { \
        void *buf; \
        if(nbuckets == head->hh_nbuckets) \
            return 0; \
        buf = allocator(alloc_size(nbuckets, head_struct)); \
        if(buf) { \
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
    extern _h_ptr * \
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
        _h_ptr *hh_buckets; \
    }

#define HASH_ENTRY(head_struct) \
    struct { \
        _h_ptr he_next; \
    }

#define HASH_PARAMS(head_struct, entry_struct, entry_field, \
                    hash_func, eq_func) \
    HASH_PARAMS_(head_struct, entry_struct, entry_field, \
                 hash_func, eq_func, \
                 static _HASH_INLINE)

#define HASH_PARAMS_EXTDEF(head_struct, entry_struct, entry_field, \
                           hash_func, eq_func) \
    HASH_PARAMS_(head_struct, entry_struct, entry_field, \
                 hash_func, eq_func, \
                 extern)

#define HASH_PARAMS_(head_struct, entry_struct, entry_field, \
                     hash_func, eq_func, \
                     func_decl) \
    typedef struct entry_struct head_struct##__hash_entry; \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_seek(const struct head_struct *head, \
                             size_t idx) { \
        _h_ptr result; \
        for(; idx < HASH_NBUCKETS(head); idx++) { \
            result = head->hh_buckets[idx]; \
            if(result) \
                return (head_struct##__hash_entry *) result; \
        } \
        return NULL; \
    } \
    static _HASH_INLINE head_struct##__hash_entry * \
    head_struct##__hash_next(const struct head_struct *head, \
                             const head_struct##__hash_entry *entry) { \
        _h_ptr next = entry->entry_field.he_next; \
        return ((size_t) next & 1) ? \
            head_struct##__hash_seek(head, (size_t) next >> 1) : \
            (head_struct##__hash_entry *) next; \
    } \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_lookup(struct head_struct *head, \
                               const head_struct##__hash_entry *proto, \
                               char remove, \
                               char add) { \
        head_struct##__hash_entry *entry; \
        _h_ptr next, *nextp; \
        size_t hash; \
        if(!HASH_NBUCKETS(head)) return NULL; \
        hash = (hash_func)(proto) % HASH_NBUCKETS(head); \
        nextp = &head->hh_buckets[hash]; \
        next = *nextp; \
        if(next == 0) \
            next = (_h_ptr) ((hash << 1) + 3); \
        if(add) { \
            *nextp = (_h_ptr) proto; \
            nextp = (_h_ptr *) &proto->entry_field.he_next; \
            *nextp = next; \
            head->hh_count++; \
        } \
        for(; \
            !((size_t) next & 1); \
            nextp = &entry->entry_field.he_next, \
            next = *nextp) { \
            unsigned char cmp; \
            entry = (head_struct##__hash_entry *) next; \
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
        return NULL; \
    } \
    _HASH_COMMON_BITS(head_struct, \
                      HASH_ALLOC_SIZE, \
                      HASH_FOREACH_SAFE, \
                      head_struct##__hash_entry *, \
                      HASH_SET, \
                      func_decl) \

#define HASH_PARAMS_EXTREF(head_struct, entry_struct, entry_field, \
                           hash_func, eq_func) \
    extern struct entry_struct * \
    head_struct##__hash_seek(const struct head_struct *head, \
                             size_t idx); \
    extern struct entry_struct * \
    head_struct##__hash_lookup(struct head_struct *head, \
                               const head_struct##__hash_entry *proto, \
                               char remove, \
                               char add); \
    _HASH_COMMON_BITS_EXTREF(head_struct)

#define HASH_INIT(head) \
    /* initialize to an empty hash */ \
    do { \
        head.hh_count = 0; \
        head.hh_nbuckets = 0; \
        head.hh_buckets = NULL; \
    } while(0)

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

#define HASH_LOOKUP(head, proto, head_struct) \
    head_struct##__hash_lookup(head, proto, 0, 0)

#define HASH_SET(head, entry, head_struct) \
    head_struct##__hash_lookup(head, entry, 1, 1)

#define HASH_REMOVE(head, proto, head_struct) \
    head_struct##__hash_lookup(head, proto, 1, 0)

#define HASH_ALLOC_SIZE(nbuckets, head_struct) \
    (sizeof(_h_ptr) * (nbuckets))

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
        
#define CHASH_PARAMS(head_struct, entry_struct, \
                     hash_func, eq_func, null_func) \
    CHASH_PARAMS_(head_struct, entry_struct, \
                  hash_func, eq_func, null_func, \
                  static _HASH_INLINE)
                  
#define CHASH_PARAMS_EXTDEF(head_struct, entry_struct, \
                            hash_func, eq_func, null_func) \
    CHASH_PARAMS_(head_struct, entry_struct, \
                  hash_func, eq_func, null_func, \
                  extern)

#define CHASH_PARAMS_(head_struct, entry_struct, \
                      hash_func, eq_func, null_func, \
                      func_decl) \
    typedef struct entry_struct head_struct##__hash_entry; \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_lookup(const struct head_struct *head, \
                               const head_struct##__hash_entry *proto, \
                               char mode) { \
        size_t hash, i, nbuckets = CHASH_NBUCKETS(head); \
        head_struct##__hash_entry \
            *bucket, *oldbucket, \
            *buckets = (head_struct##__hash_entry *) head->hh_buckets; \
        if(!nbuckets) \
            return NULL; \
        hash = i = (hash_func)(proto) % nbuckets; \
        do { \
            bucket = &buckets[i]; \
            if((null_func)(bucket)) { \
                if(mode == 1 /* add */) \
                    *bucket = *proto; \
                return NULL; \
            } \
            if((eq_func)(bucket, proto)) { \
                if(mode == 1 /* add */) \
                    *bucket = *proto; \
                else if(mode == 2 /* remove */) \
                    goto copy_backward; \
                return bucket; \
            } \
        } while((i = (i + 1) & (nbuckets - 1)) != hash); \
        return NULL; \
    copy_backward: \
        memset(bucket, 0, sizeof(*bucket)); \
        oldbucket = bucket; \
        while((i = (i + 1) & (nbuckets - 1)) != hash && \
              !(null_func)(bucket = &buckets[i]) && \
              (hash_func)(bucket) == hash) { \
            *oldbucket = *bucket; \
            memset(bucket, 0, sizeof(*bucket)); \
            oldbucket = bucket; \
        } \
        return bucket; \
    } \
    func_decl head_struct##__hash_entry * \
    head_struct##__hash_next_by_idx(const struct head_struct *head, \
                               size_t idx) { \
        size_t nbuckets = (head)->hh_nbuckets; \
        head_struct##__hash_entry *bucket = \
            &((head_struct##__hash_entry *) head->hh_buckets)[idx]; \
        for(; idx < nbuckets; idx++, bucket++) { \
            if(!null_func(bucket)) \
                return bucket; \
        } \
        return NULL; \
    } \
    static _HASH_INLINE head_struct##__hash_entry * \
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

#define CHASH_PARAMS_EXTREF(head_struct, entry_struct, \
                            hash_func, eq_func, null_func) \
    extern struct entry_struct * \
    head_struct##__hash_lookup(const struct head_struct *head, \
                               const head_struct##__hash_entry *proto, \
                               char mode); \
    extern struct entry_struct * \
    head_struct##__hash_next_by_idx(const struct head_struct *head, \
                                    size_t id); \
    _HASH_COMMON_BITS_EXTREF(head_struct)
                          
#define CHASH_HEAD_INITIALIZER HASH_HEAD_INITIALIZER

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
                            
#endif
