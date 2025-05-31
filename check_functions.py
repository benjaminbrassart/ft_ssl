#!/usr/bin/env python3

allowed_functions = {"open", "close", "read", "write", "malloc", "free"}

if __name__ == "__main__":
    import re
    import subprocess
    import sys

    args = sys.argv[1:]

    assert len(args) == 1

    p = subprocess.run(
        args=["nm", "-u", "--", args[0]],
        env={"LC_ALL": "C"},
        capture_output=True,
        text=True,
    )
    p.check_returncode()

    regex = re.compile(r"^\s+U ([^_]\w*)(?:@.+)?$")

    symbols = [
        sym.group(1)
        for sym in [regex.match(line) for line in p.stdout.splitlines()]
        if sym is not None
    ]

    for symbol in symbols:
        if symbol not in allowed_functions:
            print(symbol)
