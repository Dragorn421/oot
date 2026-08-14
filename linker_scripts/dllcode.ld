SECTIONS
{
    dll.code 0 : AT(0) {
        *(.text* .data* .rodata*)
    }
    dll.code.bss : {
        *(.bss*)
    }
}
