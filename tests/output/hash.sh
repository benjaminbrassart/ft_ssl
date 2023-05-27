#!/usr/bin/env sh

set -e

hash_real() {
    printf -- '%s' "$2" | "$1"sum | cut -d ' ' -f 1
}

hash_ft() {
    ./ft_ssl "$1" -q -s "$2"
}

output_real="$(mktemp)"
output_ft="$(mktemp)"

hash_real "$1" "$2" > "${output_real}"
hash_ft "$1" "$2" > "${output_ft}"

if diff --color=auto -- "${output_ft}" "${output_real}"; then
    printf -- 'OK.\n'
else
    printf -- '\nKO! Check diff:\n  < is your output\n  > is the expected output\n'
fi

rm "${output_real}" "${output_ft}"
