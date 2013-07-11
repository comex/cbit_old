#include "miniargp.h"
#include "stdio.h"

MARGP(argp,
    OPT('f', foo, "Test foo this is going on for very very very very very very very very very very very very very very very very long"),
    OPT('g', bar_bar, "Test bar"),
    OPTARG('h', bell, "Test bell", STR, 0),
    OPTARG('z', ball, "Test ball", INT, 0),
    OPTARGS('i', baz, "Test baz",
        ARG(baz_a0, STR, 0),
        ARG(baz_a1, INT, 1)),
    ARG(name, STR),
    ARG(things, STR_VEC),
);

int main(int argc, char **argv) {
    margp(&argp, argv);
    printf("foo=%d bar_bar=%d bell=%s ball=%d baz_a0=%s baz_a1=%d name=%s\n",
           argp.foo, argp.bar_bar, argp.bell, argp.ball, argp.baz_a0, argp.baz_a1, argp.name);
    for(char **thingp = argp.things; *thingp; thingp++) {
        printf("  thing=%s\n", *thingp);
    }
}
