import make
import os
import ast
import itertools
import functools
import operator
import json
import networkx

from typing import Union, List, Iterator, Iterable

def debug_dump(f):
    @functools.wraps(f)
    def dumper(*args, **kwargs):
        print(', '.join([f'{index}={arg}' for arg, index in zip(args, range(len(args)))]))
        print(', '.join([f'{key}={value}' for key, value in zip(kwargs.keys(), kwargs.values())]))
        return f(*args, **kwargs)
    return dumper

BuildType = Union['application', 'object', 'bundle']
ArchType = Union['x86_64', 'arm64']
ArchType = Union[ArchType, List[ArchType]]
OutputType = Union['binary', 'elf64', None]

class PykeTransform(object):
    def __init__(self, target: str, arch: ArchType, destination=None, commands=[],
                build: BuildType='application', c_files=[], c_flags=[], 
                s_files=[], s_flags=[], includes=[], l_file=None, l_flags=[], 
                dependencies=[], output: OutputType=None):
        self.architectures = [arch] if not isinstance(arch, List) else arch
        self.target = target
        self.destination = destination
        self.commands = commands
        self.c_files = c_files
        self.s_files = s_files
        self.l_file = l_file
        self.c_flags = c_flags
        self.s_flags = s_flags
        self.l_flags = l_flags
        self.dependencies = dependencies
        self.output = output
        self.includes = includes
        self.build = build

    def __repr__(self):
        return self.target

    def __hash__(self):
        return hash(self.target)

# Perform a transformation on a directory containing a Pykefile
def transform(*directories: Iterable[str]) -> List[PykeTransform]:
    def transform_collect(directory: str):
        if directory.endswith('!'):
            directory = directory[:-1]
            rest = []
        else:
            rest = map(transform_collect, filter(os.path.isdir, 
                        [f'{directory}/{dirent}' for dirent in os.listdir(directory)]))
        
        if os.path.isfile(f'{directory}/Pykefile'):
            #print(directory)
            with open(f'{directory}/Pykefile') as pykefile:
                return itertools.chain(rest, map(lambda conf: PykeTransform(**conf),
                                       ast.literal_eval(pykefile.read())))
        return rest

    def transform_flatten(iterable):
        return list(itertools.chain.from_iterable(transform_flatten(item) 
                    if isinstance(item, Iterable) else [item] for item in iterable))

    return transform_flatten(map(transform_flatten, map(transform_collect, directories)))

def build_graph(transform_list: List[PykeTransform]):
    dependency_graph = networkx.DiGraph()
    transforms = { tr.target: tr for tr in transform_list }
    dependency_graph.add_nodes_from(transform_list)
    for tr in transform_list:
        dependency_graph.add_edges_from([(tr, transforms[dep]) 
                                            for dep in tr.dependencies])
    return dependency_graph
