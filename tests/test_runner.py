import difflib
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
        id: int,
        ft_ssl: str,
        args: list[str],
        stdin: list[str] = [],
        stdout: list[str] = [],
        stderr: list[str] = [],
        files: dict[str, list[str]] = {},
        exit_code: int = 0,
        timeout: float | None = None,
        valgrind: bool = False,
    ) -> None:
        self.id = id
        self.ft_ssl = ft_ssl
        self.args = args
        self.stdin = stdin
        self.stdout = stdout
        self.stderr = stderr
        self.files = files
        self.exit_code = exit_code
        self.timeout = timeout
        self.valgrind = valgrind

    def run(self, dir: str):
        args = [self.ft_ssl] + self.args
        cmd_line = " ".join(args[1:])

        tmp = pathlib.Path(f"{dir}/{self.id}")
        tmp.mkdir(exist_ok=True)

        if self.valgrind:
            args = [
                "valgrind",
                "--show-leak-kinds=all",
                "--leak-check=full",
                "--track-fds=yes",
                "--track-origins=yes",
                "--error-exitcode=42",
                "--xml=yes",
                f"--xml-file={tmp}/valgrind.xml",
                f"--log-file={tmp}/valgrind.log",
                "--",
            ] + args

        os.chdir(tmp)

        for file_name, content in self.files.items():
            with open(file_name, "w") as f:
                f.writelines([line + "\n" for line in content])

        p = subprocess.Popen(
            args,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd=tmp,
        )

        stdout, stderr = p.communicate(
            bytes("\n".join(self.stdin), "utf-8"), timeout=self.timeout
        )

        with open("command", "wt") as f:
            print(cmd_line, file=f)

        if p.returncode != self.exit_code:
            eprint("exit code mismatch:")
            eprint(f"  want = {self.exit_code}")
            eprint(f"  have = {p.returncode}")

        diff = list(
            difflib.unified_diff(
                str(stdout, "utf-8").splitlines(), self.stdout, lineterm=""
            )
        )

        if len(diff) != 0:
            with open(f"{tmp}/stdin", "wt") as f:
                f.write("\n".join(self.stdin))

            with open(f"{tmp}/stdout.diff", "wt") as f:
                print("\n".join(diff), file=f)

            with open(f"{tmp}/stdout.have", "wb") as f:
                f.write(stdout)

            with open(f"{tmp}/stdout.want", "wt") as f:
                f.write("\n".join(self.stdout))

            with open(f"{tmp}/stderr", "wb") as f:
                f.write(stderr)

        # shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    import argparse
    import json
    import os

    parser = argparse.ArgumentParser(prog="test_runner.py")

    parser.add_argument("--timeout", type=float, default=1.0)
    parser.add_argument("--binary", type=pathlib.Path, default="./ft_ssl")
    parser.add_argument("--valgrind", action="store_true")
    parser.add_argument("files", nargs="+")

    args = parser.parse_args()

    ft_ssl = str(args.binary.absolute())

    tmp = pathlib.Path(tempfile.mkdtemp(dir="tests", prefix="_ft_ssl_test_"))
    tests = []
    status = 0

    for arg in args.files:
        try:
            with open(arg, "rt") as f:
                data = json.load(f)
                assert type(data) is list, f"expected list, got {type(data)}"

                for test_dict in data:
                    assert (
                        type(test_dict) is dict
                    ), f"expected dict, got {type(test_dict)}"

                    id = len(tests)
                    tests.append(
                        Test(
                            id=id,
                            ft_ssl=ft_ssl,
                            timeout=args.timeout,
                            valgrind=args.valgrind,
                            **test_dict,
                        )
                    )

        except Exception as e:
            eprint(f"{e}")
            status = 1

    for test in tests:
        test.run(tmp)

    exit(status)
