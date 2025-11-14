from setuptools import setup, Extension
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
LIB_DIR = os.path.join(BASE_DIR, "..", "lib")

module = Extension(
    "sparse",
    sources=[
        "src/wrapper.c",
        os.path.join(LIB_DIR, "src", "handler.c"),
        os.path.join(LIB_DIR, "src", "arg.c"),
    ],
    include_dirs=[
        os.path.join(LIB_DIR, "include"),
    ],
    extra_compile_args=["-Wall", "-O2"],
)

setup(ext_modules=[module])
