#!/bin/bash
set -e
hashtest_expected="{
foo=derp
mia=fey
}
{
foo=derp
mia=fey
}
{
foo=derp
mia=fey
}
{
foo=derp
mia=fey
}
{
mia=fey
}
{
mia=fey
}
n{
foo=derp
mia=fey
} 2
n{
foo=derp
mia=fey
} 2
n{
mia=fey
} 1
n{
mia=fey
} 1"

preproctest_expected="pass
pass
<1> <2> <3> <4> <5>"

margptest_help_expected="Usage: ./margptest [OPTION]... [name] [things]...
  -f, --foo                      Test foo this is going on for very very
                                 very very very very very very very very
                                 very very very very very very long
  -g, --bar-bar                  Test bar
  -h, --bell [val]               Test bell
  -z, --ball [val]               Test ball
  -i, --baz [baz-a0] [baz-a1]    Test baz
  --help                         This help message"

margptest_opts_expected="foo=1 bar_bar=0 bell=1 ball=2 baz_a0=(null) baz_a1=0 name=a
  thing=b
  thing=a"

hashtest() {
    for compiler in clang++ g++ g++-mp-4.8; do
        for mode in "-x c -ansi" \
                    "-x c -std=c99" \
                    "-x c -std=gnu99" \
                    "-x c++ -std=c++98" \
                    "-x c -ansi -DHASH_FORCE_VP_HASH_PTR=1"; do
            for closed in 1 0; do \
                for ext in 1 0; do
                    x="$compiler $mode -DCLOSED=$closed -DEXT=$ext -Wall -Wextra -Werror -o hashtest hashtest.c"
                    echo "$x"
                    $x
                    diff_cmd "./hashtest" "$hashtest_expected"
                    echo "pass"
                done
            done
        done
    done
}

preproctest() {
    for compiler in clang gcc-mp-4.8; do
        x="$compiler -P -E preproctest.h"
        echo "$x"
        diff_cmd "$x" "$preproctest_expected"
        echo "pass"
    done
}

margptest() {
    for mode in "-x c -ansi" \
                "-x c -std=c99" \
                "-x c++ -std=c++98"; do
        x="g++-mp-4.8 -Wall -Wextra -Werror $mode -o margptest margptest.c"
        echo "$x"
        $x
        diff_cmd "./margptest --help" "$margptest_help_expected"
        echo "pass"
    done
    diff_cmd "./margptest --foo a b a -hz 1 2" "$margptest_opts_expected"
    echo "pass"
}

diff_cmd() {
    real="$($1 | grep -v '^ *$' | sed 's/ *$//')"
    if [ "$2" != "$real" ]; then
        echo "Difference when running $1: expected {"
        echo "$2"
        echo "} but got {"
        echo "$real"
        echo "}"
        exit 1
    fi
}

case "$1" in
    -preproc) preproctest ;;
    -hash) hashtest ;;
    -margp) margptest ;;
    "") preproctest; hashtest; margptest ;;
    *) echo "Usage: ./test.sh [-preproc | -hash | -margp]" >&2 ;;
esac
