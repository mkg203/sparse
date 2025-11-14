from setuptools import setup, Extension
import os

module = Extension(
    "sparse",
    sources=["src/wrapper.c", "../lib/src/handler.c", "../lib/src/arg.c"],
    include_dirs=[
        os.path.join("../lib/include")
    ],
)

setup(
    name="sparse",
    version="0.1.0",
    ext_modules=[module],
)
