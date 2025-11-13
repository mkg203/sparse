from setuptools import setup, Extension

test= Extension(
    'test',
    sources=['src/test_module.c'],
)

setup(
    name='test',
    version='0.1',
    ext_modules=[test],
)
