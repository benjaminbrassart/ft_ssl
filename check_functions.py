#!/usr/bin/env python3

allowed_functions = {"open", "close", "read", "write", "malloc", "free"}

if __name__ == "__main__":
    import subprocess
    import sys

    args = sys.argv[1:]

    assert len(args) == 1

    p = subprocess.run(
        args=["nm", "-u", "-f", "just-symbols", "--", args[0]],
        env={"LANG": "C"},
        capture_output=True,
        text=True,
    )
    p.check_returncode()

    symbols = [
        symbol.partition("@")[0]
        for symbol in p.stdout.splitlines()
        if not symbol.startswith("_")
    ]
    forbidden_symbols = [
        symbol for symbol in symbols if symbol not in allowed_functions
    ]

    if len(forbidden_symbols) != 0:
        print("\n".join(forbidden_symbols))
