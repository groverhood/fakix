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


def main(args: List[str]):
    parser = argparse.ArgumentParser(prog='pyke', description='pythonic makefile generator')
    parser.add_argument('--arch', '-a', type=str, dest='arch', default='x86_64', choices=['arm64', 'x86_64'])
    parser.add_argument('--source', '-s', type=str, dest='source', default='.')

    noentry = frozenset(['.git', 'cross', 'pyke', 'tools', 'errors', 'include'])
    caneval = funcy.compose(operator.__not__, noentry.__contains__)
    res = parser.parse_args(args)
    transforms = pyke.transform(f'{res.source}!', *filter(caneval, filter(os.path.isdir, 
                                os.listdir(res.source))))

    dep_graph = pyke.build_graph(transforms)
        

if __name__ == '__main__':
    main(sys.argv[1:])