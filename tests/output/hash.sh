#!/usr/bin/env sh

set -e

rcut() {
    rev | cut "$@" | rev
}

hash_real() {
    printf -- '%s' "$2" | openssl "$1" | rcut -d ' ' -f 1
}

hash_ft() {
    printf -- '%s' "$2" | ./ft_ssl "$1" | rcut -d ' ' -f 1
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

rm -f "${output_real}" "${output_ft}"
