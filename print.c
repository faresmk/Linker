#include "projet.h"
#include "read_rel.c"



// print Elf32_Ehdr
void print_header(FILE *fp){
    Elf32_Ehdr *header = malloc(sizeof(Elf32_Ehdr));
    *header = read_header(fp);

    //print header
    printf("En-tête ELF:\n");
    printf("  Magique:   ");
    for (int i = 0; i < EI_NIDENT; i++)
        printf("%02x ", header->e_ident[i]);
    printf("\n");
    printf("  Classe:                            ELF32\n");
    printf("  Données:                          complément à 2, système à octets de poids faible d'abord (little endian)\n");
    printf("  Version:                           1 (actuelle)\n");
    printf("  OS/ABI:                            UNIX - System V\n");
    printf("  Version ABI:                       0\n");
    printf("  Type:                              ");
    switch (header->e_type){   
        case ET_NONE: printf("Aucun type de fichier\n"); 
            break;                     
        case ET_REL: printf("Fichier réadressable\n");
            break;  
        case ET_EXEC: printf("Fichier exécutable\n");
            break;
        case ET_DYN: printf("Fichier objet partagé\n");
            break;
        case ET_CORE: printf("fichier core\n");
            break;
        case ET_LOPROC: printf("spécifique au processeur\n");
            break;
        case ET_HIPROC: printf("spécifique au processeur\n");
            break;  
        default: printf("Type de fichier inconnu\n");
            break;
    }
    printf("  Machine:                           ");
    switch(header->e_machine){
        case EM_NONE: printf("Plateforme cible inconnue\n");
            break;
        case EM_M32: printf("Plateforme cible : AT&T WE 32100\n");
            break;
        case EM_SPARC: printf("Plateforme cible : SPARC\n");
            break;
        case EM_386: printf("Plateforme cible : Intel Architecture\n");
            break;
        case EM_68K: printf("Plateforme cible : Motorola 68000\n");
            break;
        case EM_88K: printf("Plateforme cible : Motorola 88000\n");
            break;
        case EM_860: printf("Plateforme cible : Intel 80860\n");
            break;
        case EM_MIPS: printf("Plateforme cible : MIPS RS3000 Big-Endian\n");
            break;
        case EM_MIPS_RS3_LE: printf("Plateforme cible : MIPS RS4000 Big-Endian\n");
            break;
        case EM_ARM: printf("Plateforme cible : ARM\n");
            break;
        default: printf("Plateforme cible : Reserved for future use\n");
            break;
    }
    printf("  Version:                           0x%d\n", header->e_version);
    // switch (header->e_version){
    // case EV_NONE:
    //     printf("Version invalide.\n");
    //     break;
    // default:
    //     printf("Version actuelle.\n");
    //     break;
    // }
    printf("  Adresse du point d'entrée:         0x%08x\n", header->e_entry);
    printf("  Début des en-têtes de programme :  %d (octets dans le fichier)\n", header->e_phoff);
    printf("  Début des en-têtes de section :    %d (octets dans le fichier)\n", header->e_shoff);
    printf("  Fanions:                           0x%08x\n", header->e_flags);
    printf("  Taille de cet en-tête:             %d (octets)\n", header->e_ehsize);
    printf("  Taille de l'en-tête du programme:  %d (octets)\n", header->e_phentsize);
    printf("  Nombre d'en-tête du programme:     %d\n", header->e_phnum);
    printf("  Taille des en-têtes de section:    %d (octets)\n", header->e_shentsize);
    printf("  Nombre d'en-têtes de section:      %d\n", header->e_shnum);
    printf("  Table d'index des chaînes d'en-tête de section: %d\n", header->e_shstrndx);
}
// print section
void print_section(FILE *fp){
    int i;
    Elf32_Ehdr *header = malloc(sizeof(Elf32_Ehdr));
    *header = read_header(fp);
    Elf32_Shdr *section = malloc(sizeof(Elf32_Shdr) * header->e_shnum);
    printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%x:\n", header->e_shnum, header->e_shoff);
    printf("\n");
    printf("En-têtes de section : \n");
    printf("  [Nr] Nom                Type              Adr      Décala Taille ES Fan LN Inf Al \n");
    for (int i = 0; i < header->e_shnum; i++){
        section[i] = read_section(fp, header, section, i);
    }
    for (int i = 0; i < header->e_shnum; i++){
        printf("  [%2d] ", i);
        printf("%-18s ", get_section_name(fp, header, section, i));
        printf("%-18s ", get_section_type(fp, header, section, i));
        printf("%08x ", section[i].sh_addr);
        printf("%06x ", section[i].sh_offset);
        printf("%06x ", section[i].sh_size);
        printf("%02x ", section[i].sh_entsize);
        printf("%3s ", get_section_flags(fp, header, section, i));
        printf("%2d ", section[i].sh_link);
        printf("%3d ", section[i].sh_info);
        printf("%2d ", section[i].sh_addralign);
        printf("\n");
    }
    free(header);
    free(section);
}


void print_content_section(FILE *fp){
    Elf32_Ehdr *header = malloc(sizeof(Elf32_Ehdr));
    *header = read_header(fp);
    Elf32_Shdr *section = malloc(sizeof(Elf32_Shdr) * header->e_shnum);
    for (int i = 0; i < header->e_shnum; i++){
        section[i] = read_section(fp, header, section, i);
    }
    int i;
    printf("Pour afficher le contenu d'une section, taper un nombre entre 0 et %d\n", header->e_shnum - 1);
    scanf("%d", &i);
    if (i >= 0 && i < header->e_shnum){
        display_section_content(fp, header, section, i);
    }else{
        printf("Erreur de saisie");
    }
    free(header);
    free(section);
}



//  Function to print the symbole table
void print_symbole(FILE *fp){
    Elf32_Ehdr *header;
    header = malloc(sizeof(Elf32_Ehdr));
    *header = read_header(fp);
    Elf32_Shdr *section;
    section = malloc(sizeof(Elf32_Shdr) * header->e_shnum);
    Elf32_Sym symbole;
    printf("Num:   Valeur     Tail Type    Lien    Vis     Ndx Nom\n");
    for (int i = 0; i < header->e_shnum; i++){
        section[i] = read_section(fp, header, section, i);
    }
    for (int i = 0; i < header->e_shnum; i++){
        if (section[i].sh_type == SHT_SYMTAB){
            for (int j = 0; j < section[i].sh_size / section[i].sh_entsize; j++){
                symbole = read_symbole(fp, header, section, j);
                printf("  %3d: ", j);
                printf("%08x ", symbole.st_value);
                printf("%6d ", symbole.st_size);
                switch (ELF32_ST_TYPE(symbole.st_info)){
                case STT_NOTYPE:
                    printf("NOTYPE  ");
                    break;
                case STT_OBJECT:
                    printf("OBJECT  ");
                    break;
                case STT_FUNC:
                    printf("FUNC    ");
                    break;
                case STT_SECTION:
                    printf("SECTION ");
                    break;
                case STT_FILE:
                    printf("FILE    ");
                    break;
                case STT_LOPROC:
                    printf("LOPROC  ");
                    break;
                case STT_HIPROC:
                    printf("HIPROC  ");
                    break;
                default:
                    printf("Unknown ");
                    break;
                }
                switch (ELF32_ST_BIND(symbole.st_info)){ // 0:LOCAL 1:GLOBAL 2:WEAK
                case STB_LOCAL:
                    printf("LOCAL   ");
                    break;
                case STB_GLOBAL:
                    printf("GLOBAL  ");
                    break;
                case STB_WEAK:
                    printf("WEAK    ");
                    break;
                case STB_LOPROC:
                    printf("LOPROC  ");
                    break;
                case STB_HIPROC:
                    printf("HIPROC  ");
                    break;
                default:
                    printf("Unknown ");
                    break;
                }
                switch (symbole.st_other){
                case STV_DEFAULT:
                    printf("DEFAULT ");
                    break;
                case STV_INTERNAL:
                    printf("INTERNAL ");
                    break;
                case STV_HIDDEN:
                    printf("HIDDEN ");
                    break;
                case STV_PROTECTED:
                    printf("PROTECTED ");
                    break;
                default:
                    printf("Unknown ");
                    break;
                }
                switch (symbole.st_shndx){
                case SHN_UNDEF:
                    printf("UND ");
                    break;
                case SHN_ABS:
                    printf("ABS ");
                    break;
                case SHN_COMMON:
                    printf("COM ");
                    break;
                default:
                    printf("%3d ", symbole.st_shndx);
                    break;
                }
                printf("%s", read_string(fp, header, section, symbole.st_name));
                printf("\n");
            }
        }
    }
    free(section);
    free(header);
}
// get print relocations
void print_rel(FILE *fp){
    Elf32_Ehdr *header;
    header = malloc(sizeof(Elf32_Ehdr));
    *header = read_header(fp);
    Elf32_Shdr *section;
    section = malloc(sizeof(Elf32_Shdr) * header->e_shnum);
    Elf32_Rel rel;
    
    for (int i = 0; i < header->e_shnum; i++){
        section[i] = read_section(fp, header, section, i);
        if (section[i].sh_type == SHT_REL)
        { 
            printf("Section %d \n", i);
            printf("Décalage Info     Type            \n");
                for (int j = 0; j < section[i].sh_size / section[i].sh_entsize; j++){
                    rel = read_rel(fp, header, section, j);
                    printf("%08x ", rel.r_offset);
                    printf("%08x ", rel.r_info);
                    printf("%-14s", get_relocation_type(ELF32_R_TYPE(rel.r_info)));
                    printf("\n");
                }
        }
    }
    free(header);
    free(section);
}