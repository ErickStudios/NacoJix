
#include "../include/jix.h"

typedef unsigned int u32;
typedef unsigned short u16;

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

static volatile u32 tty_row;
static volatile u32 tty_col;
static volatile u16* const VGA = (volatile u16*)0xB8000;

void JixMemoryInstall();
static void tty_lineskip() {
    tty_row++;
    tty_col = 0;
}

static void print(const char* str)
{
    static unsigned int pos = 0;

    while (*str)
    {
        char ttyr = *str;
        if (tty_col >= 80)
            tty_lineskip();

        if (ttyr == '\n') tty_lineskip();
        else { 
            VGA[(tty_row * 80) + tty_col] = (u16)(0x0200 | (unsigned char)ttyr);
            tty_col++;
        }

        *str++;
    }
}

static void jix_install_tty() {
    tty_row = 0;
    tty_col = 0;
} /*Acabo de instalar unos mods de ttyr en el Servidor de objetos
Ahhyyy bro, que hijo de puta genio eres!!! puede mostrar texto?
si*/

void jix_main(u32 magic, u32 mbi_addr)
{
    (void)mbi_addr;
    jix_install_tty();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        print("JIX: Invalid bootloader!");
        return;
    }

    JixMemoryInstall();
    print("JIX Kernel");

    METATABLE* MyTable = JixTable();

    SetProperty(MyTable, "MyName", META_POINTER, (METATHING){
        .pointer = "Hello World"
    });

    METAENTRY* name = GetProperty(MyTable, "MyName");

    if (name)
        print((const char*)name->anyThing.pointer);
}
