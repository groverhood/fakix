PHDRS
{
  headers PT_PHDR PHDRS;
  interp PT_INTERP;
  text PT_LOAD FILEHDR PHDRS;
  data PT_LOAD;
  dynamic PT_DYNAMIC;
}

SECTIONS
{
  . = SIZEOF_HEADERS;
  .interp : { *(.interp) } :text :interp
  .text : { *(.text) } :text
  .rodata : { *(.rodata) }
  .data : { *(.data) } :data
  .bss : { *(.bss) }
  .dynamic : { *(.dynamic) } :dynamic
}