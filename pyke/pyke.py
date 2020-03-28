import make
import os
import ast
import itertools
import functools
import operator
import json
import networkx
import funcy
import re

from enum import Enum
from typing import Union, List, Iterator, Iterable

def debug_dump(f):
    @functools.wraps(f)
    def dumper(*args, **kwargs):
        print(', '.join([f'{index}={arg}' for arg, index in zip(args, range(len(args)))]))
        print(', '.join([f'{key}={value}' for key, value in zip(kwargs.keys(), kwargs.values())]))
        return f(*args, **kwargs)
    return dumper


def transform_flatten(iterable):
    return list(itertools.chain.from_iterable(transform_flatten(item) 
                if isinstance(item, Iterable) and not isinstance(item, str) else [item] for item in iterable))

class Architecture(object):

    def __init__(self, arch_type: str):
        prefix = {
            'x86_64': 'x86_64-linux-gnu',
            'arm64': 'aarch64-linux-gnu'
        }[arch_type]

        self.arch_type = arch_type
        self.compiler = f'{prefix}-gcc-9 -c -nostdlib'
        self.linker = f'{prefix}-ld'
        self.assember = f'{prefix}-as'
        self.preprocessor = f'{prefix}-cpp-9'

    def __str__(self):
        return self.arch_type

    def __repr__(self):
        return self.arch_type

    def __eq__(self, oth):
        return self.arch_type == oth.arch_type if isinstance(oth, Architecture) else self.arch_type == oth

    def __ne__(self, oth):
        return self.arch_type != oth.arch_type if isinstance(oth, Architecture) else self.arch_type != oth

    def attrgetter(self, name):
        return functools.partial(getattr, self, name)

    @staticmethod
    def generate_arch_list(*arch_types):
        return list(map(Architecture, arch_types))

class PykeTransform(object):
    @staticmethod
    def __format(source_root: str, local_root: str, file: str):
        return (f'{local_root}/{file}' if '{source_root}' not in file
                        else file.format(source_root=source_root))

    def __init__(self, target: str, arch: str, destination=None, commands=[],
                build: str='application', c_files=[], c_flags=[], 
                s_files=[], s_flags=[], includes=[], l_file=None, l_flags=[], 
                dependencies=[], output: str=None, source_root=None,
                local_root=None):

        assert source_root is not None and local_root is not None
        bound_format = functools.partial(PykeTransform.__format, source_root,
                                         local_root)
        self.architectures = Architecture.generate_arch_list(*([arch] if not isinstance(arch, list) else arch))
        self.selected_arch: Architecture = None
        self.target = target
        self.destination = destination
        self.commands = commands
        self.c_files = list(map(bound_format, c_files))
        self.s_files = list(map(bound_format, s_files))
        self.l_file = bound_format(l_file) if l_file is not None else None
        self.c_flags = ' '.join(c_flags)
        self.s_flags = ' '.join(s_flags)
        self.l_flags = ' '.join([*l_flags, *(['-T build/generated/' + os.path.basename(self.l_file)] if l_file is not None else [])])
        self.dependencies = dependencies
        self.output = output
        self.includes = ' '.join(map(funcy.compose(functools.partial(operator.__concat__, '-I'), bound_format),
                                     includes))
        self.build = build
        self.binding_dgraph: networkx.DiGraph = None
        self.generated_make = False
        self.comp = False
        self.comp_objs = False

    @staticmethod
    def build_string(s: str):
        return 'build/obj/' + re.sub(r'\.[sc]$', '.o', os.path.basename(s))

    def depobjs(self):
        return map(PykeTransform.objs, filter(funcy.compose(functools.partial(operator.__eq__, 'object'),
                funcy.rpartial(getattr, 'build')), map(operator.itemgetter(1), self.binding_dgraph.out_edges(self))))
 

    def objs(self):
        if not self.comp_objs:
            self.objects = transform_flatten(itertools.chain(map(PykeTransform.build_string, self.c_files), 
                                             map(PykeTransform.build_string, self.s_files), self.depobjs()))
            self.comp_objs = True
        return self.objects

    def __to_str(self):
        if self.comp:
            return ''

        def link_body():
            if self.build == 'bundle':
                return []

            objstring = ' '.join(self.objs())
            l_flags = self.l_flags + (f' -r -o build/target/{self.target}.o' if self.build == 'object' else f' -o build/target/{self.target}')
            ld_command = f'{self.selected_arch.linker} {l_flags} {objstring}'
            return ([f'{self.selected_arch.preprocessor} {self.includes} {self.l_file} | grep -v \'^#\' > build/generated/' + os.path.basename(self.l_file) + f'; {ld_command}']
                        if self.l_file is not None 
                        else [f'{self.selected_arch.linker} {l_flags} {objstring}'])

        self.comp = True
        if not self.generated_make:
            edges = map(funcy.rpartial(PykeTransform.bind, self.binding_dgraph, self.selected_arch),
                        [v for u, v in self.binding_dgraph.out_edges(self)])
            depstring = ' '.join(self.dependencies)
            s_files = [(PykeTransform.build_string(s_file), s_file) for s_file in self.s_files]
            c_files = [(PykeTransform.build_string(c_file), c_file) for c_file in self.c_files]
            self.make = '\n'.join([*filter(len, map(str, edges)), 
                                   '\n\t'.join([f'{self.target}: {depstring}',
                                              *itertools.starmap(lambda o, c: f'{self.selected_arch.compiler} {self.c_flags} {self.includes} {c} -o {o}', c_files),
                                              *itertools.starmap(lambda o, s: f'{self.selected_arch.preprocessor} -DASM_FILE {s} {self.includes} | {self.selected_arch.assember} {self.s_flags} -o {o}',
                                                   s_files),
                                              *link_body()])])
            self.generated_make = True
    
        return self.make    

    def bind(self, dep_graph: networkx.DiGraph, selected_arch: str):
        self.binding_dgraph = dep_graph
        self.selected_arch = self.architectures[self.architectures.index(selected_arch)]
        return self

    def __str__(self):
        return self.__to_str() if self.binding_dgraph is not None else self.target

    def __repr__(self):
        return self.target

    def __hash__(self):
        return hash(self.target)

# Perform a transformation on a directory containing a Pykefile
def transform(source_root: str, *directories: Iterable[str]) -> List[PykeTransform]:

    def transform_collect(directory: str):
        if source_root == directory:
            rest = []
        else:
            rest = map(transform_collect, filter(os.path.isdir, 
                        [f'{directory}/{dirent}' for dirent in os.listdir(directory)]))
        
        if os.path.isfile(f'{directory}/Pykefile'):
            with open(f'{directory}/Pykefile') as pykefile:
                return itertools.chain(rest, map(lambda conf: PykeTransform(**conf,
                                                 source_root=source_root, 
                                                 local_root=directory),
                                       ast.literal_eval(pykefile.read())))
        return rest


    return [*transform_flatten(map(funcy.compose(transform_flatten, transform_collect),
            filter(os.path.isdir, map(functools.partial(operator.__concat__, f'{source_root}/'), 
            directories)))), *transform_flatten(transform_collect(source_root))]

def build_graph(transform_list: List[PykeTransform]):
    dependency_graph = networkx.DiGraph()
    transforms = { tr.target: tr for tr in transform_list }
    dependency_graph.add_nodes_from(transform_list)
    for tr in transform_list:
        dependency_graph.add_edges_from([(tr, transforms[dep]) 
                                            for dep in tr.dependencies])
    return dependency_graph

def generate_makefile(arch: str, dag: networkx.DiGraph):
    def get_arch_recipes(arch: str, dag: networkx.DiGraph):
        """Get all recipes compatible with the desired build architecture"""
        return filter(funcy.compose(funcy.rpartial(operator.__contains__, arch),
                        funcy.rpartial(getattr, 'architectures')), 
                        filter(funcy.compose(functools.partial(operator.__eq__, 0),
                            dag.in_degree), 
                            dag.nodes))

    def get_default_rules():
        return [make.MakeRule('all', deps=list(map(funcy.rpartial(getattr, 'target'), get_arch_recipes(arch, dag)))),
                make.MakeRule('clean', body=['rm -rf build']), 
                make.MakeRule('builddir', body=['mkdir -p build build/target build/obj build/generated'])]
    
    _, *_ = map(funcy.rpartial(PykeTransform.bind, dag, arch), dag)

    return map(str, itertools.chain(get_default_rules(), get_arch_recipes(arch, dag)))
