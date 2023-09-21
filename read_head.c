#include "projet.h"

// read  header
Elf32_Ehdr read_header(FILE *fp)
{
    Elf32_Ehdr ehdr;
    fread(&ehdr, sizeof(Elf32_Ehdr), 1, fp);
    return ehdr;
}
