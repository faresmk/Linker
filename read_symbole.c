#include "projet.h"
#include "read_section.c"

//Function to read the symbole table
Elf32_Sym read_symbole(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i){
    Elf32_Sym symbole;
    for (int j = 0; j < header->e_shnum; j++){
        if (section[j].sh_type == SHT_SYMTAB){
            fseek(fp, section[j].sh_offset + i * section[j].sh_entsize, SEEK_SET);
            fread(&symbole, sizeof(Elf32_Sym), 1, fp);
        }
    }
    return symbole;
}

// Function to read the string table
char *read_string(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i){
    char *string;
    for (int j = 0; j < header->e_shnum; j++){
        if (section[j].sh_type == SHT_STRTAB){
            string = malloc(sizeof(char) * section[j].sh_size);
            fseek(fp, section[j].sh_offset, SEEK_SET);
            fread(string, sizeof(char), section[j].sh_size, fp);
            return string + i;
        }
    }
    return NULL;
}