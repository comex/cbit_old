#include "preproc.h"
#define foo(ctx, x) <x>
PP_CHOOSE(, fail, pass)
PP_CHOOSE(foo, pass, fail)
PP_APPLY(foo, 0, 1, 2, 3, 4, 5)
PP_APPLY(foo, 0)
