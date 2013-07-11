#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "preproc.h"
#include <limits.h>

#define MARGP_LINE_LEN 72

enum margp_meta_kind {
    margp_meta_opt,
    margp_meta_end,
    margp_meta_optarg,
    margp_meta_arg,
};

enum margp_meta_type {
    margp_meta_type_STR,
    margp_meta_type_INT,
    margp_meta_type_STR_VEC
};

struct margp_meta {
    enum margp_meta_kind kind;
    union {
        struct margp_meta_opt {
            char chr;
            char *name;
            const char *desc;
            size_t offset;
        } opt;
        struct margp_meta_arg {
            enum margp_meta_type type;
            char *name;
            size_t offset;
        } arg;
    } u;
};

#define MARGP_DECL_TYPE_STR char *
#define MARGP_DECL_TYPE_INT int
#define MARGP_DECL_TYPE_STR_VEC char **

#define MARGP_X_OPT(...) (OPT, __VA_ARGS__)
#define MARGP_X_OPTARG(...) (OPTARG, __VA_ARGS__)
#define MARGP_X_OPTARGS(...) (OPTARGS, __VA_ARGS__)
#define MARGP_X_ARG(...) (ARG, __VA_ARGS__)

#define MARGP_DECL(sname, bits) PP_UNWRAP(MARGP_DECL_, MARGP_X_, sname, bits)
#define MARGP_DECL_OPT(sname, chr, name, desc) \
    bool name;
#define MARGP_DECL_OPTARG(sname, chr, name, desc, type, defl) \
    MARGP_DECL_TYPE_##type name;
#define MARGP_DECL_OPTARGS(sname, chr, name, desc, ...) \
    PP_APPLY2(MARGP_DECL_OPT_ARG, sname, __VA_ARGS__)
#define MARGP_DECL_ARG(sname, name, type) \
    MARGP_DECL_TYPE_##type name;
#define MARGP_DECL_OPT_ARG(sname, bits) PP_UNWRAP2(MARGP_DECL_OPT_ARG_, MARGP_X_, sname, bits)
#define MARGP_DECL_OPT_ARG_ARG(sname, name, type, defl) \
    MARGP_DECL_TYPE_##type name;

#define MARGP_META(sname, bits) PP_UNWRAP(MARGP_META_, MARGP_X_, sname, bits)
#define MARGP_META_OPT(sname, chr, name, desc) \
    {margp_meta_opt, {.opt = {chr, #name, desc, \
                      offsetof(sname, name)}}},
#define MARGP_META_OPTARG(sname, chr, name, desc, type, defl) \
    {margp_meta_opt, {.opt = {chr, #name, desc, 0}}}, \
    {margp_meta_optarg, {.arg = {margp_meta_type_##type, #name, \
                     offsetof(sname, name)}}},
#define MARGP_META_OPTARGS(sname, chr, name, desc, ...) \
    {margp_meta_opt, {.opt = {chr, #name, desc, 0}}}, \
    PP_APPLY2(MARGP_META_OPT_ARG, sname, __VA_ARGS__)
#define MARGP_META_ARG(sname, name, type) \
    {margp_meta_arg, {.arg = {margp_meta_type_##type, #name, \
                              offsetof(sname, name)}}},
#define MARGP_META_OPT_ARG(sname, bits) PP_UNWRAP2(MARGP_META_OPT_ARG_, MARGP_X_, sname, bits)
#define MARGP_META_OPT_ARG_ARG(sname, name, type, defl) \
    {margp_meta_optarg, {.arg = {margp_meta_type_##type, #name, \
                      offsetof(sname, name)}}},

#define MARGP_DEFAULT(sname, bits) PP_UNWRAP(MARGP_DEFAULT_, MARGP_X_, sname, bits)
#define MARGP_DEFAULT_OPT(sname, chr, name, desc) \
    false,
#define MARGP_DEFAULT_OPTARG(sname, chr, name, desc, type, defl) \
    defl,
#define MARGP_DEFAULT_OPTARGS(sname, chr, name, desc, ...) \
    PP_APPLY2(MARGP_DEFAULT_OPT_ARG, sname, __VA_ARGS__)
#define MARGP_DEFAULT_ARG(sname, name, type) \
    0,
#define MARGP_DEFAULT_OPT_ARG(sname, bits) PP_UNWRAP2(MARGP_DEFAULT_OPT_ARG_, MARGP_X_, sname, bits)
#define MARGP_DEFAULT_OPT_ARG_ARG(sname, name, type, defl) \
    defl,

#define MARGP(name, ...) \
    struct margp_##name { \
        struct margp_meta *_meta; \
        PP_APPLY(MARGP_DECL, struct margp_##name, __VA_ARGS__) \
        bool help; \
    }; \
    struct margp_meta margp_meta_##name[] = { \
        PP_APPLY(MARGP_META, struct margp_##name, __VA_ARGS__) \
        {margp_meta_end, {.opt = {0, "help", "This help message", \
                                  offsetof(struct margp_##name, help)}}},  \
    }; \
    struct margp_##name name = { \
        margp_meta_##name, \
        PP_APPLY(MARGP_DEFAULT, struct margp_##name, __VA_ARGS__) \
        0 \
    }

static void margp_usage(struct margp_meta *meta, char **argv) {
    struct margp_meta *m;
    int maxlen;

    printf("Usage: %s [OPTION]...", argv[0]);
    for(m = meta; m->kind != margp_meta_end; m++) {
        if(m->kind == margp_meta_arg) {
            struct margp_meta_arg *a = &m->u.arg;
            printf(" [%s]", a->name);
            if(a->type == margp_meta_type_STR_VEC) {
                printf("...");
            }
        }
    }
    // xx: args
    printf("\n");

    maxlen = 0;
    for(m = meta; ; m++) {
        struct margp_meta_opt *o;
        char *left, *lp;
        int len;
        if(m->kind > margp_meta_end)
            continue;
        o = &m->u.opt;
        lp = left = malloc(128);
        if(o->chr)
            lp += sprintf(lp, "-%c", o->chr);
        if(o->name) {
            if(o->chr)
                lp += sprintf(lp, ", ");
            lp += sprintf(lp, "--%s", o->name);
        }
        o->name = left;
        len = lp - left;
        if(len > maxlen) maxlen = len;
        if(m->kind == margp_meta_end)
            break;
    }
    maxlen += 4;

    for(m = meta; ; m++) {
        struct margp_meta_opt *o;
        const char *dp;
        char c;
        if(m->kind > margp_meta_end)
            continue;
        o = &m->u.opt;
        printf("  %-*s", maxlen, o->name);
        if(o->desc) {
            int linelen = 2 + maxlen;
            for(dp = o->desc; (c = *dp); dp++) {
                if(c == ' ') {
                    char *next_space = strchr(dp + 1, ' ');
                    if(linelen > 2 + maxlen &&
                       next_space && linelen + (next_space - dp) > MARGP_LINE_LEN)
                        c = '\n';
                }
                putchar(c);
                linelen++;
                if(c == '\n') {
                    printf("  %-*s", maxlen, "");
                    linelen = 2 + maxlen;
                }
            }
        }
        printf("\n");
        if(m->kind == margp_meta_end)
            break;
    }

}

static void margp_fix_dash(char **namep) {
    if(*namep) {
        char *name = malloc(strlen(*namep)), *n;
        const char *p;
        for(p = *namep, n = name; *p; p++, n++) {
            *n = (*p == '_') ? '-' : *p;
        }
        *namep = name;
    }
}

static size_t margp_init(struct margp_meta *meta) {
    struct margp_meta *m;
    for(m = meta; ; m++) {
        if(m->kind == margp_meta_arg) {
            margp_fix_dash(&m->u.arg.name);
        } else if(m->kind <= margp_meta_end) {
            margp_fix_dash(&m->u.opt.name);
            if(m->kind == margp_meta_end)
                return m->u.opt.offset;
        }
    }
}

struct margp_str_vec {
    char **args;
    size_t len;
    size_t capacity;
};

static void margp_str_vec_append(struct margp_str_vec *str_vec, char *arg) {
    if(++str_vec->len > str_vec->capacity) {
        size_t new_capacity = 2 * str_vec->capacity + 4;
        str_vec->args = realloc(str_vec->args, new_capacity * sizeof(char *));
    }
    str_vec->args[str_vec->len - 1] = arg;
}

static struct margp_meta *margp_parse_arg(char *arg, void *margp, struct margp_meta *m, struct margp_str_vec *str_vec) {
    struct margp_meta_arg *ma = &m->u.arg;
    void *ap = (char *) margp + ma->offset;
    long l;
    char *endptr;
    switch(ma->type) {
    case margp_meta_type_STR:
        *(char **) ap = arg;
        break; 
    case margp_meta_type_INT:
        l = strtol(arg, &endptr, 0);
        if(*endptr) {
            fprintf(stderr, "Error: bad integer %s\n", arg);
            exit(1);
        }
        if(l < INT_MIN || l > INT_MAX) {
            fprintf(stderr, "Error: integer out of range: %s\n", arg);
            exit(1);
        }
        *(int *) ap = l;
        break;
    case margp_meta_type_STR_VEC:
        margp_str_vec_append(str_vec, arg);
        return m;
    default:
        abort();
    }
    return m + 1;
}

static bool margp_parse_opt(char ***argvp, char **pendingp, void *margp, struct margp_meta *m) {
    struct margp_meta_opt *o = &m->u.opt;
    if(o->offset) {
        *(bool *) ((char *) margp + o->offset) = true;
    }
    while((++m)->kind == margp_meta_optarg) {
        char *arg = *pendingp ? *pendingp : *(*argvp)++;
        if(!arg) {
            fprintf(stderr, "Error: Not enough arguments\n");
            return true;
        }
        *pendingp = 0;
        margp_parse_arg(arg, margp, m, 0);
    }
    return false;
}

static bool margp_run(void *margp, struct margp_meta *meta, char **argv) {
    char *arg, *pending = 0;
    struct margp_meta *arg_m = meta;
    struct margp_str_vec str_vec = {0, 0, 0};
    char ***str_vec_outp = 0;
    bool dashdash = false;
    argv++;
    while((arg = pending ? pending : *argv++)) {
        pending = 0;
        if(arg[0] != '-' || dashdash) {
            // an argument
            if(arg_m->kind != margp_meta_arg) {
                do {
                    if(arg_m->kind == margp_meta_end) {
                        fprintf(stderr, "Error: Too many arguments (saw <%s>)\n", arg);
                        exit(1);
                    }
                    arg_m++;
                } while(arg_m->kind != margp_meta_arg);
            }
            arg_m = margp_parse_arg(arg, margp, arg_m, &str_vec);
        } else {
            // an option
            if(arg[1] == '-') {
                if(arg[2] == '\0') {
                    dashdash = true;
                } else {
                    // one long option
                    struct margp_meta *m;
                    char *eq;
                    if((eq = strstr(arg, "="))) {
                        pending = eq + 1;
                        *eq = 0;
                    }
                    for(m = meta; ; m++) {
                        struct margp_meta_opt *o;
                        if(m->kind > margp_meta_end)
                            continue;
                        o = &m->u.opt;
                        if(!strcmp(o->name, arg + 2)) {
                            if(margp_parse_opt(&argv, &pending, margp, m))
                                return true;
                            break;
                        }
                        if(m->kind == margp_meta_end) {
                            fprintf(stderr, "Error: Unknown option: %s\n", arg);
                            exit(1);
                        }
                    }
                }
            } else {
                // one or more short options
                // note that we support weird syntax because why not:
                //   -ab a_arg b_arg
                const char *ap;
                char c;
                for(ap = arg + 1; (c = *ap); ap++) {
                    struct margp_meta *m;
                    for(m = meta; ; m++) {
                        struct margp_meta_opt *o;
                        if(m->kind > margp_meta_end)
                            continue;
                        o = &m->u.opt;
                        if(o->chr == c) {
                            if(margp_parse_opt(&argv, &pending, margp, m))
                                return true;
                            break;
                        }
                        if(m->kind == margp_meta_end) {
                            fprintf(stderr, "Error: Unknown option: -%c\n", c);
                            exit(1);
                        }
                    }
                }
            }
        }
    }
    while(arg_m->kind != margp_meta_end) {
        if(arg_m->kind == margp_meta_arg &&
           arg_m->u.arg.type == margp_meta_type_STR_VEC) {
            str_vec_outp = (char ***) ((char *) margp + arg_m->u.arg.offset);
        }
        if((++arg_m)->kind == margp_meta_arg) {
            fprintf(stderr, "Error: Not enough arguments\n");
            return true;
        }
    }
    if(str_vec_outp) {
        margp_str_vec_append(&str_vec, 0);
        *str_vec_outp = str_vec.args;
    }
    return false;
}

static void margp(void *margp, char **argv) {
    struct margp_meta *meta = *(struct margp_meta **) margp;
    size_t offset = margp_init(meta);
    bool need_help = margp_run(margp, meta, argv);
    bool *help_p = (bool *) ((char *) margp + offset);
    if(need_help || *help_p) {
        margp_usage(meta, argv);
        exit(1);
    }
}

