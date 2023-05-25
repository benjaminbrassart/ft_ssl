#!/usr/bin/env sh

make -j || exit 2

STATUS=0

printf -- '\nRunning tests:\n'

for f in $(find . -name '*.out'); do
    basename -z "${f}" .out
    printf -- ' -> '
    if "$f"; then
        printf -- 'OK!\n'
    else
        STATUS=1
    fi
done

exit "${STATUS}"
