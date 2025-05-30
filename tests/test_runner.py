import functools
import pathlib
import shutil
import subprocess
import sys
import tempfile

eprint = functools.partial(print, file=sys.stderr)


class Test:
    def __init__(
        self,
        ft_ssl: str,
        args: list[str],
        stdin: list[str] = [],
        stdout: list[str] = [],
        stderr: list[str] = [],
        files: dict[str, list[str]] = {},
        exit_code: int = 0,
        timeout: float | None = None,
    ) -> None:
        self.ft_ssl = ft_ssl
        self.args = args
        self.stdin = stdin
        self.stdout = stdout
        self.stderr = stderr
        self.files = files
        self.exit_code = exit_code
        self.timeout = timeout

    def run(self):
        tmp = pathlib.Path(tempfile.mkdtemp(prefix="_ft_ssl_test_"))
        args = [self.ft_ssl] + self.args
        cmd_line = " ".join(args[1:])
        eprint(f"ft_ssl {cmd_line}")

        os.chdir(tmp)

        for file_name, content in self.files.items():
            with open(file_name, "w") as f:
                f.writelines(content)

        p = subprocess.Popen(
            args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd=tmp,
        )

        _, _ = p.communicate(timeout=self.timeout)

        if p.returncode != self.exit_code:
            eprint("exit code mismatch:")
            eprint(f"  want = {self.exit_code}")
            eprint(f"  have = {p.returncode}")

        # shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    import json
    import os

    args = sys.argv[1:]

    if len(args) == 0:
        raise  # TODO print usage

    ft_ssl = pathlib.Path(os.environ.get("FT_SSL", "./ft_ssl")).absolute()
    timeout = float(os.environ.get("TEST_TIMEOUT", "1"))
    status = 0

    tests = []

    for arg in args:
        try:
            with open(arg, "rt") as f:
                data = json.load(f)
                assert type(data) is list, f"expected list, got {type(data)}"

                for test_dict in data:
                    assert (
                        type(test_dict) is dict
                    ), f"expected dict, got {type(test_dict)}"

                    tests.append(Test(ft_ssl=str(ft_ssl), timeout=timeout, **test_dict))

        except Exception as e:
            eprint(f"{e}")
            status = 1

    for test in tests:
        test.run()

    exit(status)
