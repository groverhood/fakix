
import operator as __op
import funcy as __fn
import os as __os
import typing as __typing

def makeconfig(sb: __typing.Union[str, __typing.Iterable[str]]):
    def __setup_config(sb: __typing.Iterable[str]):
        sbins = frozenset(sb)
        multiboot_entries = frozenset(sbin for sbin in sbins if 'cpu_driver' in sbin)
        multiboot_entry, = multiboot_entries
        modules = sbins.difference(multiboot_entries)
        module_string = '\n'.join(map(__fn.partial(__op.__concat__, 'module2 '), modules)) if len(modules) else ''
        return f'''
set timeout=0
set default=0

menuentry "Fakix" {{
    multiboot2 {multiboot_entry}
    {module_string}
    boot
}}
'''

    return __setup_config(sb if not isinstance(sb, str)
                                else filter(__os.path.isfile, __os.listdir(sb)))
