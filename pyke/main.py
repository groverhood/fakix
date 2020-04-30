import sys
import argparse
import make
import pyke
import os
import ast
import itertools
import operator
import funcy

from typing import List

workspace_root = ''

def main(args: List[str]):
    parser = argparse.ArgumentParser(prog='pyke', description='pythonic makefile generator')
    parser.add_argument('--arch', '-a', type=str, dest='arch', default='x86_64', choices=['aarch64', 'x86_64'])
    parser.add_argument('--source', '-s', type=str, dest='source', default='.')
    res = parser.parse_args(args)

    if os.path.abspath(res.source) == os.path.abspath(os.getcwd()):
        raise Exception('idiot')

    os.system('rm -rf ./*')

    noentry = frozenset(['.git', 'cross', 'pyke', 'errors', 'include'])
    caneval = funcy.compose(operator.__not__, noentry.__contains__)
    transforms = pyke.transform(res.source, *filter(caneval, os.listdir(res.source)))
    dep_graph = pyke.build_graph(res.arch, transforms)
    makefile, sbins = pyke.generate_makefile(res.arch, dep_graph)

    for builddir in ['build/target', 'build/obj', 'build/generated', 'boot/BOOTBOOT',
                     'initrd', 'tools']:
        try:
            os.makedirs(builddir)
        except FileExistsError:
            continue

    if res.arch == 'x86_64':
        os.makedirs('boot/EFI/BOOT')
    
    with open('Makefile', 'w') as makefile_stream:
        makefile_stream.write('\n'.join(makefile))

if __name__ == '__main__':
    main(sys.argv[1:])