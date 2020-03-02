#ifndef ELF_EUTIL_H
#define ELF_EUTIL_H 1

#include <stddef.h>
#include <elf/elf.h>
#include <fakix/addr.h>
#include <fakix/errtype.h>

typedef errval_t elf_alloc_fn_t(void *state, vaddr_t addr, size_t size, void **ret);

errval_t elf_load(void *ehdr, size_t esize, elf_alloc_fn_t *fun, void **ret_entry);

errval_t elf32_load(Elf32_Ehdr *ehdr, size_t esize, elf_alloc_fn_t *fun, void **ret_entry);
errval_t elf64_load(Elf64_Ehdr *ehdr, size_t esize, elf_alloc_fn_t *fun, void **ret_entry);

#endif