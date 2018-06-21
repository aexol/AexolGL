import sys
import os
from os.path import join, realpath, dirname, exists
from shutil import rmtree, copytree
import imp
import compileall

sys.path = [join(realpath(dirname(__file__)), 'site-packages')] + sys.path
import click
from dirsync import sync

@click.command()
@click.option('--sdk', help="SDK path.")
@click.option('--src', help="Source directory.")
@click.option('--out', default="", help="Output path for compilation.")
def compile(sdk, src, out):
    # If out set, copy source and compile it outside of source directory,
    # else do it in place.
    if out:
        dst = out
        sync(src, dst, "sync", create=True)
    else:
        dst = src

    with open(join(sdk, '.python_magic')) as f:
        python_magic = f.readline()
    if python_magic != imp.get_magic().encode('hex'):
        return 1
    compileall.compile_dir(dst)
    return 0

if __name__ == '__main__':
    compile()
