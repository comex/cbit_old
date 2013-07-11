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

diff_cmd() {
    real="$($1 | grep -v '^$' | sed 's/ *$//')"
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
    "") preproctest; hashtest ;;
    *) echo "Usage: ./test.sh [-preproc | -hash]" >&2 ;;
esac
