
.section .multiboot
.align 4

.set MB_MAGIC, 0x1BADB002
.set MB_FLAGS, 0x00000003
.set MB_CHECKSUM, -(MB_MAGIC + MB_FLAGS)

.long MB_MAGIC
.long MB_FLAGS
.long MB_CHECKSUM

.section .text
.global _start
.extern jix_main

_start:
    cli

    # Preparar stack del kernel
    mov $stack_top, %esp

    # Argumentos Multiboot para C
    push %ebx
    push %eax

    call jix_main

    add $8, %esp

halt:
    cli
    hlt
    jmp halt

.section .bss
.align 16

stack_bottom:
    .skip 16384
stack_top:
