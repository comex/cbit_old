#include "margp.h"
#include "stdio.h"

bool foo, bar_bar;
char *bell, *baz_a0, *name, **things;
int ball, baz_a1;

struct margp_meta argp[] = {
    MARGP_OPT0('f', "foo", "Test foo this is going on for very very very very very very very very very very very very very very very very long", &foo),
    MARGP_OPT0('g', "bar-bar", "Test bar", &bar_bar),
    MARGP_OPT1('h', "bell", "Test bell", margp_str, &bell),
    MARGP_OPT1('z', "ball", "Test ball", margp_int, &ball),
    MARGP_OPTN('i', "baz", "Test baz"),
        MARGP_OPTARG("baz-a0", margp_str, &baz_a0),
        MARGP_OPTARG("baz-a1", margp_int, &baz_a1),
    MARGP_ARG("name", margp_str, &name),
    MARGP_ARG("things", margp_str_vec, &things),
    MARGP_END
};

int main(int argc, char **argv) {
    (void) argc;
    char **thingp;
    margp(argp, argv);
    printf("foo=%d bar_bar=%d bell=%s ball=%d baz_a0=%s baz_a1=%d name=%s\n",
           foo, bar_bar, bell, ball, baz_a0, baz_a1, name);
    for(thingp = things; *thingp; thingp++) {
        printf("  thing=%s\n", *thingp);
    }
    return 0;
}
