SECTIONS
{
    dll.code : {
        *(.text* .data* .rodata*)
    }
    dll.code.bss : {
        *(.bss*)
    }
}
