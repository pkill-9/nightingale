ENTRY("NIGHTINGALE");
OUTPUT_FORMAT("elf32-i386")
SECTIONS
{
    . = 0x00100000;

    .text . : 
    {
        *(.text);
    }

    .data . :
    {
        *(.bss);
        *(.bss*);
        *(.data);
        *(.rodata*);
        *(COMMON);
        STACK_BASE = 0x0007FFFF;
    }

    /DISCARD/ :
    {
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.comment);
    }
}
