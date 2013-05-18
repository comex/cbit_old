set -e
expected="{
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


for compiler in clang++ g++ g++-mp-4.8; do
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
