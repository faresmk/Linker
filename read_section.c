#include "projet.h"
#include "read_head.c"
// read section (it needs an index of which section to read)
Elf32_Shdr read_section(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i)
{
    fseek(fp, header->e_shoff + i * header->e_shentsize, SEEK_SET);
    fread(section + i, sizeof(Elf32_Shdr), 1, fp);
    return section[i];
}
// get section name
char *get_section_name(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i)
{
    fseek(fp, section[header->e_shstrndx].sh_offset + section[i].sh_name, SEEK_SET);
    char *name = malloc(sizeof(char) * 20);
    fread(name, sizeof(char), 20, fp);
    return name;
}
// get section type
char *get_section_type(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i)
{
    char *type = malloc(sizeof(char) * 20);
    switch (section[i].sh_type)
    {
    case SHT_NULL:
        strcpy(type, "NULL");
        break;
    case SHT_PROGBITS:
        strcpy(type, "PROGBITS");
        break;
    case SHT_SYMTAB:
        strcpy(type, "SYMTAB");
        break;
    case SHT_STRTAB:
        strcpy(type, "STRTAB");
        break;
    case SHT_RELA:
        strcpy(type, "RELA");
        break;
    case SHT_HASH:
        strcpy(type, "HASH");
        break;
    case SHT_DYNAMIC:
        strcpy(type, "DYNAMIC");
        break;
    case SHT_NOTE:
        strcpy(type, "NOTE");
        break;
    case SHT_NOBITS:
        strcpy(type, "NOBITS");
        break;
    case SHT_REL:
        strcpy(type, "REL");
        break;
    case SHT_SHLIB:
        strcpy(type, "SHLIB");
        break;
    case SHT_DYNSYM:
        strcpy(type, "DYNSYM");
        break;
    case SHT_INIT_ARRAY:
        strcpy(type, "INIT_ARRAY");
        break;
    case SHT_FINI_ARRAY:
        strcpy(type, "FINI_ARRAY");
        break;
    case SHT_PREINIT_ARRAY:
        strcpy(type, "PREINIT_ARRAY");
        break;
    case SHT_GROUP:
        strcpy(type, "GROUP");
        break;
    case SHT_SYMTAB_SHNDX:
        strcpy(type, "SYMTAB_SHNDX");
        break;
    case SHT_NUM:
        strcpy(type, "NUM");
        break;
    case SHT_LOOS:
        strcpy(type, "LOOS");
        break;
    case SHT_GNU_ATTRIBUTES:
        strcpy(type, "GNU_ATTRIBUTES");
        break;
    case SHT_GNU_HASH:
        strcpy(type, "GNU_HASH");
        break;
    case SHT_GNU_LIBLIST:
        strcpy(type, "GNU_LIBLIST");
        break;
    case SHT_CHECKSUM:
        strcpy(type, "CHECKSUM");
        break;
    case SHT_LOSUNW:
        strcpy(type, "LOSUNW");
        break;
    case SHT_SUNW_COMDAT:
        strcpy(type, "SUNW_COMDAT");
        break;
    case SHT_SUNW_syminfo:
        strcpy(type, "SUNW_syminfo");
        break;
    case SHT_GNU_verdef:
        strcpy(type, "GNU_verdef");
        break;
    case SHT_GNU_verneed:
        strcpy(type, "GNU_verneed");
        break;
    case SHT_GNU_versym:
        strcpy(type, "GNU_versym");
        break;
    case SHT_LOPROC:    
        strcpy(type, "LOPROC");
        break;
    case SHT_HIPROC:
        strcpy(type, "HIPROC");
        break;
    case SHT_LOUSER:
        strcpy(type, "LOUSER");
        break;
    case SHT_HIUSER:
        strcpy(type, "HIUSER");
        break;
    default:
        strcpy(type, "ARM_ATTRIBUTES");
        break;
    }
    return type;
}




// get section flags
char *get_section_flags(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i)
{
    char *flags = malloc(sizeof(char) * 20);
    strcpy(flags, " ");
    if (section[i].sh_flags & SHF_WRITE)
        strcat(flags, "W");
    if (section[i].sh_flags & SHF_ALLOC)
        strcat(flags, "A");
    if (section[i].sh_flags & SHF_EXECINSTR)
        strcat(flags, "X");
    if (section[i].sh_flags & SHF_MERGE)
        strcat(flags, "M");
    if (section[i].sh_flags & SHF_STRINGS)
        strcat(flags, "S");
    if (section[i].sh_flags & SHF_INFO_LINK)
        strcat(flags, "I");
    if (section[i].sh_flags & SHF_LINK_ORDER)
        strcat(flags, "L");
    if (section[i].sh_flags & SHF_OS_NONCONFORMING)
        strcat(flags, "O");
    if (section[i].sh_flags & SHF_GROUP)
        strcat(flags, "G");
    if (section[i].sh_flags & SHF_TLS)
        strcat(flags, "T");
    if (section[i].sh_flags & SHF_MASKOS)
        strcat(flags, "o");
    if (section[i].sh_flags & SHF_MASKPROC)
        strcat(flags, "p");
    if (section[i].sh_flags & SHF_ORDERED)
        strcat(flags, "E");
    if (section[i].sh_flags & SHF_EXCLUDE)
        strcat(flags, "X");
    return flags;
}


//dispay section content
void display_section_content(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i)
{
    int j;
    unsigned char *content = malloc(sizeof(char) * section[i].sh_size);
    fseek(fp, section[i].sh_offset, SEEK_SET);
    fread(content, sizeof(char), section[i].sh_size, fp);
    for (j = 0; j < section[i].sh_size; j++)
    {
        if (j % 16 == 0)
            {
                printf("\n");
                printf("0x%08x ", section[i].sh_addr +j);
            }
        if (j % 4 == 0)
            printf(" ");
        printf("%02x", content[j]);
    }
    printf("\n");
}

