set -e
expected="{
foo=derp
mia=fey
}
{
foo=derp
mia=fey
}"


for compiler in clang++ g++; do
    for mode in "-x c -ansi" \
                "-x c -std=c99" \
                "-x c -std=gnu99" \
                "-x c++ -std=c++98"; do
        for closed in 1 0; do \
            for ext in 1 0; do
                x="$compiler $mode -DCLOSED=$closed -DEXT=$ext -Wall -Wextra -Werror -o hashtest hashtest.c"
                echo "$x"
                $x
                diff -q <(./hashtest) <(echo "$expected")
                echo "pass"
            done
        done
    done
done
