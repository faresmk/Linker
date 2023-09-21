// fonction qui prend en argument un fichier ELF, et qui affiche sur la sortie standard la plateforme cible et la taille des mots (32/64 bits) de l'en-tête d'un fichier ELF
// le fichier ELF est passé en argument de la fonction
// le fichier ELF est ouvert en lecture seule
// le fichier ELF est fermé à la fin de la fonction
// la fonction retourne 0 si tout s'est bien passé, 1 sinon
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "elf.h" // on inclut le fichier elf.h qui contient les structures et les constantes nécessaires à la lecture de l'en-tête ELF

int displayELF(char *fichier){
    // on ouvre le fichier ELF en lecture
    FILE* fd = popen(fichier, "r");
    if (fd == NULL){
        perror("open");// perror sert à afficher le message d'erreur associé à la dernière erreur rencontrée par le programme
        pclose(fd);
        return 1;
    }
    /* on récupère la taille du fichier ELF
    struct stat st;
    stat(argv[1], &st);
    int size = st.st_size;
    */

    // on vérifie que le fichier ELF est bien un fichier ELF
    char sortie[1024];
    fgets(sortie, sizeof(sortie), fd);// fgets lit une ligne d'un fichier et la stocke dans une chaîne de caractères

    // vérifie si la sortie contient la chaîne "ELF"
    if (strstr(sortie, "ELF") == NULL){
        printf("Ce n'est pas un fichier ELF.\n");
        return 1;
    }

    //on récupère l'en-tête du fichier ELF dans la variable header
    Elf64_Ehdr header;
    if (fread(&header, sizeof(Elf32_Ehdr), 1, fd) != 1){// fread lit des données depuis un fichier et les stocke dans un tableau de données (ici header)
        perror("Erreur lors de la lecture de l'en-tête.\n");
        pclose(fd);
        return -1;
    }
    //on affiche la plateforme cible à partir du header header
    printf("Plateforme cible : \n");
    switch(header.e_machine){
        case EM_NONE:
            printf("Plateforme cible inconnue.\n");
            break;
        case EM_M32:
            printf("Plateforme cible : AT&T WE 32100\n");
            break;
        case EM_SPARC:
            printf("Plateforme cible : SPARC\n");
            break;
        case EM_386:
            printf("Plateforme cible : Intel Architecture\n");
            break;
        case EM_68K:
            printf("Plateforme cible : Motorola 68000\n");
            break;
        case EM_88K:
            printf("Plateforme cible : Motorola 88000\n");
            break;
        case EM_860:
            printf("Plateforme cible : Intel 80860\n");
            break;
        case EM_MIPS:
            printf("Plateforme cible : MIPS RS3000 Big-Endian\n");
            break;
        case EM_MIPS_RS3_LE:
            printf("Plateforme cible : MIPS RS4000 Big-Endian\n");
            break;
        default:
            printf("Plateforme cible : Reserved for future use\n");
            break;
    }
    // on affiche le type de fichier ELF (fichier relogeable, fichier exécutable...)
    printf("Type de fichier : ");
    switch (header.e_type){   
        case ET_NONE:
            printf("No file type.\n");
            break;
        case ET_REL:
            printf("Relocatable file.\n");
            break;  
        case ET_EXEC:
            printf("Executable file.\n");
            break;
        case ET_DYN:
            printf("Shared object file.\n");
            break;
        case ET_CORE:
            printf("Core file.\n");
            break;
        case ET_LOPROC:
            printf("Processor-specific.\n");
            break;
        case ET_HIPROC:
            printf("Processor-specific.\n");
            break;  
        default:
            printf("Type de fichier inconnu.\n");
            break;
    }
    // on affiche la spécification de la table des sections : position dans le fichier (offset), taille globale et nombre d'entrées
    printf("Spécification de la table des sections :\n");
    printf("Position dans le fichier : %ld (bytes)\n", header.e_shoff);
    printf("Taille globale : %d (bytes)\n", header.e_shentsize);
    printf("Nombre d'entrées : %d\n", header.e_shnum);
    // on affiche l'index de l'entrée correspondant à la table des chaînes de noms de sections au sein de la table des sections
    printf("Index de l'entrée correspondant à la table des chaînes de noms de sections au sein de la table des sections : %d\n", header.e_shstrndx);
    // on affiche la taille de l'en-tête
    printf("Taille de l'en-tête : %d (bytes).\n", header.e_ehsize);
    
    // Etape 2 : on affiche la table des sections
    char nom[100];
    char section_type[100];
    char section_addr[100];
    char section_offset[100];
    char section_size[100];
    char section_EntSize[50];
    char section_flags[50];
    char section_link[50];
    char section_info[50];
    char section_align[50];
    printf("Sections headers :\n");
    printf("[Nr] Nom\t\tType\t\tAdr\t\tOff\t\tSize\t\tES\tFlg\tLk\tInf\tAl\n");
    // on se place au début de la table des sections
    fseek(fd, header.e_shoff, SEEK_SET);// fseek permet de se déplacer dans un fichier (ici on se déplace au début de la table des sections)
    // on parcourt la table des sections. e_shnum est le nombre d'entrées de la table des sections
    for (int i = 0; i < header.e_shnum; i++){
        // on récupère l'entrée de la table des sections
        Elf64_Shdr section;
        if (fread(&section, sizeof(Elf64_Shdr), 1, fd) != 1){
            perror("Erreur lors de la lecture de l'entrée de la table des sections.\n");
            pclose(fd);
            return -1;
        }
        // fseek permet de se déplacer dans un fichier (ici on se déplace au début de la table des chaînes de noms de sections)
        // e_shoff contient l'offset de la table des sections (position dans le fichier)
        // e_shstrndx contient l'index de l'entrée correspondant à la table des chaînes de noms de sections dans la table des sections
        // e_shentsize contient la taille d'une entrée de la table des sections
        // sh_name contient l'offset de la chaîne de caractères dans la table des chaînes de noms de sections
        // sh_type contient le type de la section
        // sh_addr contient l'adresse de la section
        // sh_offset contient l'offset de la section
        // sh_size contient la taille de la section
        // sh_entsize contient la taille d'une entrée de la section
        // sh_flags contient les flags de la section
        // sh_link contient l'index de la table des chaînes de noms de sections
        // sh_info contient l'index de la table des chaînes de noms de sections
        // sh_addralign contient l'alignement de la section      
        printf("[%d] %s\t\t%d\t\t0x%08x\t%d\t\t%d\t\t%d\t%d\t%d\t%d\t%d\n", i, shdr.sh_name, shdr.sh_type, shdr.sh_addr, shdr.sh_offset, shdr.sh_size, shdr.sh_entsize, shdr.sh_flags, shdr.sh_link, shdr.sh_info, shdr.sh_addralign);
    }
    //on ferme le fichier ELF
    if (pclose(fd) == -1){
        perror("close");
        return 1;
    }
    return 0;  
}

// Etape 3 : on affiche le contenu d’une section
// exemple : readelf -x .text fichier.elf (affiche la sextion .text)
// on récupère le nom de la section à afficher
int displaySection(char* fichier, char* section){
    // on ouvre le fichier ELF en lecture
    FILE* fd = popen(fichier, "r");
    if (fd == NULL){
        perror("open");// perror sert à afficher le message d'erreur associé à la dernière erreur rencontrée par le programme
        pclose(fd);
        return 1;
    }
    // on vérifie que le fichier ELF est bien un fichier ELF
    char sortie[1024];
    fgets(sortie, sizeof(sortie), fd);// fgets lit une ligne d'un fichier et la stocke dans une chaîne de caractères

    // vérifie si la sortie contient la chaîne "ELF"
    if (strstr(sortie, "ELF") == NULL){
        printf("Ce n'est pas un fichier ELF.\n");
        return 1;
    }

    //on récupère l'en-tête du fichier ELF dans la variable header
    Elf64_Ehdr header;
    if (fread(&header, sizeof(Elf32_Ehdr), 1, fd) != 1){// fread lit des données depuis un fichier et les stocke dans un tableau de données (ici header)
        perror("Erreur lors de la lecture de l'en-tête.\n");
        pclose(fd);
        return -1;
    }
    // on affiche le contenu de la section section
    // on se place au début de la table des sections
    fseek(fd, header.e_shoff, SEEK_SET);// fseek permet de se déplacer dans un fichier (ici on se déplace au début de la table des sections)
    // on parcourt la table des sections. e_shnum est le nombre d'entrées de la table des sections
    for (int i = 0; i < header.e_shnum; i++){
        // on récupère l'entrée de la table des sections
        Elf64_Shdr section;
        if (fread(&section, sizeof(Elf64_Shdr), 1, fd) != 1){
            perror("Erreur lors de la lecture de l'entrée de la table des sections.\n");
            pclose(fd);
            return -1;
        }
        // on vérifie si le nom de la section correspond à la section à afficher
        if (strcmp(section.sh_name, &section) == 0){
            // on se place au début de la section
            fseek(fd, section.sh_offset, SEEK_SET);
            // on affiche le contenu de la section
            for (int j = 0; j < section.sh_size; j++){
                printf("%02x ", fgetc(fd));
            }
            printf("\n");
            break;
        }
    }
    // on ferme le fichier ELF
    if (pclose(fd) == -1){
        perror("close");
        return 1;
    }
    return 0;
}

// Etape 4 : on affiche la table des symboles
// exemple : readelf -s fichier.elf (affiche la table des symboles)
int displaySymb(){
    return 0;
}

int main(int argc, char** argv){
    if (argc != 2 || argc != 3){
        printf("Nombre d'arguments incorrect. Veillez donner 2 ou 3 arguments.\n");
        printf("Usage : %s fichier.elf\n", argv[0]);
        printf("Pour afficher une section particulière, utiliser la commande : %s fichier.elf -x <nom section> %ss\n", argv[0], argv[1], argv[2]);
        printf("Exemple : %s fichier.elf -x .text\n", argv[0]);
        return 1;
    }
    if (argc == 2)
        return displayELF(argv[1]);
    else if (argc == 3)
        return displaySection(argv[1], argv[2]);
    return 0;
}