from typing import Union, Sequence, Dict
import ast
import functools

class MakeVariable(object):
    def __init__(self, name: str, value = None):
        self.name = name
        self.value = value

    def reference(self):
        return f'$({self.name})'

    def set_value(self, value):
        self.value = value

    def __str__(self):
        return '' if self.value is None else str(self.value)

    def __repr__(self):
        return f'{self.name}={self}'

    def __hash__(self):
        return hash(self.name)

MakeValue = Union[str, MakeVariable]

class MakeRule(object):
    def __init__(self, target: MakeValue, deps: Sequence[MakeValue] = [], body: Sequence[MakeValue] = []):
        self.target = target
        self.deps = deps
        self.body = body
        self.str = None

    def add_dep(self, dep: MakeValue):
        self.deps.append(dep)

    def set_body(self, body: Sequence[MakeValue]):
        self.body = body

    def __str__(self):
        if self.str is None:
            self.str = (self.target + ': ' + ' '.join(self.deps) + 
                '\n\t' + '\n\t'.join(self.body))
        return self.str
