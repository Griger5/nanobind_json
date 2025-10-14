from contextlib import contextmanager
from pathlib import Path
import os

@contextmanager
def __build_extension_env():
    cookies = []
    # https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
    if hasattr(os, "add_dll_directory"):
        basepath = os.path.dirname(os.path.abspath(__file__))
        dllspath = os.path.join(basepath, "..")
        os.environ["PATH"] = dllspath + os.pathsep + os.environ["PATH"]
        for path in os.environ.get("PATH", "").split(os.pathsep):
            if path and Path(path).is_absolute() and Path(path).is_dir():
                cookies.append(os.add_dll_directory(path))
    try:
        yield
    finally:
        for cookie in cookies:
            cookie.close()

with __build_extension_env():
    from _test_json_ext import *