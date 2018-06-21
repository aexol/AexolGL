import subprocess
from os.path import join, realpath, dirname, exists
from os import walk, chdir
from shutil import copy2
import sys
from zipfile import ZipFile
sys.path = [join(realpath(dirname(__file__)), 'site-packages')] + sys.path
from helper_utils import compare_files, make_dir
import click

def get_compiled_source_list(src_path):
    src_list = []
    chdir(src_path)
    for dp, dns, fns in walk('.'):
        dp = dp[2:]
        filtered = []
        for fn in fns:
            if fn.endswith('.pyo') or fn.endswith('.so'):
                if not fn.startswith('test_'):
                    filtered.append(fn)
        src_list.append((dp , filtered))
    return src_list


@click.command()
@click.option('--sdk', help="SDK path.")
@click.option('--src', help="Source directory.")
@click.option('--out', help="iPhone zip target path.")
def compile(sdk, src, out):
    host_python = join(sdk, 'bin', 'host_python')
    compile_script = join(dirname(realpath(__file__)), 'compile_android.py')
    compile_cmd = [host_python, '-OO', compile_script, '--src', src, '--sdk', sdk]
    subprocess.check_call(compile_cmd)
    update_archive = False
    pyo_list = get_compiled_source_list(src)
    for dp, fns in pyo_list:
        make_dir(join(out, dp))
        for fn in fns:
            in_file = join(dp, fn)
            out_file = join(out, dp, fn)

            # Check if python archive needs update.
            if fn.endswith('.pyo') and not compare_files(in_file, out_file):
                copy2(in_file, out_file)

            if fn.endswith('.so'):
                open(out_file, 'a').close()

if __name__ == '__main__':
    compile()
