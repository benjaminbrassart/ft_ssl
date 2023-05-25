#!/usr/bin/env sh

for f in $(find . -name '*.out'); do
    basename -z "${f}" .out
    printf -- ' -> '
    if "$f"; then
        printf -- 'OK!\n'
    fi
done
