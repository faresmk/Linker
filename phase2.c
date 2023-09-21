#include "projet.h"
#include "print.c"

int nb_elements = 0;

// structure représentant une section d'un fichier elf
typedef struct section_elf{
    int numero_section;
    int nom_section;
    unsigned char *contenu_section;
    int taille_section;
    int type_section;
    int offset_section;
    Elf32_Addr adresse_section;
    char *chaine;
}section_elf;

//déclare structure contenant  des structures section_elf
typedef struct tab_section_elf{
    section_elf *tableau_sections;
    int taille;
}tab_section_elf;

/// copie result est une copie d'une fonction : https://stackoverflow.com/questions/29079011/copy-file-function-in-c
int copie_result (char FileSource [], char FileDestination [])
{
    int   c;
    FILE *fp;
    FILE *result; 

    fp = fopen (FileSource, "r");
    if (fp == NULL)
        return -1;
    result = fopen (FileDestination, "w");   //create and write to file
    if (result == NULL)
     {
        fclose (fp);
        return -2;
     }    
    while ((c = fgetc(fp)) != EOF)
        fputc (c, result);
    fclose (fp);
    fclose (result);
    
    return 0;
}
//////////////////////////////////////////////////////


void affiche_section(section_elf section){
    printf("Contenu de la section %d \n", section.nom_section);
    printf("%s",section.chaine);
    printf("\n");
}

void affiche_structure(tab_section_elf *tab_section_fp){
    for (int i=0; i<tab_section_fp->taille; i++){
        affiche_section(tab_section_fp->tableau_sections[i]);
    }
}

//fonction qui vérifie si un numero est dans une des structures section_elf d'une structure tab_section_elf
int index_numero_identique(tab_section_elf *tab_section_fp, int number){
    for (int i=0; i<tab_section_fp->taille; i++){
            if (tab_section_fp->tableau_sections[i].numero_section==number){
                return i;
            }
        }
    return -1;
}

//fonction qui vérifie si un entier est dans une des structures section_elf d'une structure tab_section_elf
int appartient_structure(tab_section_elf *tab_section_fp, int nom_section){
    for (int i=0; i<tab_section_fp->taille; i++){
        if (tab_section_fp->tableau_sections[i].nom_section==nom_section){
            return i;//renvoie l'indice de la structure section_elf dans la structure tab_section_elf
        }
    }
    return -1;//renvoie -1 si l'entier n'est pas dans la structure
}

void ajout_structure(tab_section_elf *tab_section_fp, section_elf section_fp){
    tab_section_fp->taille+=1;
    tab_section_fp->tableau_sections= realloc(tab_section_fp->tableau_sections, tab_section_fp->taille * sizeof(section_elf));
    tab_section_fp->tableau_sections[tab_section_fp->taille-1]=section_fp;  
}

void concatene_section_progbits(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, tab_section_elf *tab_section_fp){
    static char chaine[10000];
    static char chaine2[10000];
    unsigned char *buffer;
    int indice_section;
    if (nb_elements==0){
        nb_elements = header->e_shnum;
    }
    
    for(int number = 0; number<header->e_shnum ;number++){
        fseek(fp, header->e_shoff + number * header->e_shentsize, SEEK_SET);//déplace le pointeur de fichier au début de la section
        fread(section, sizeof(Elf32_Shdr), 1, fp);//read the section header
        fseek(fp, section->sh_offset, SEEK_SET);//déplace le pointeur de fichier au début du contenu de la section. SEEK_SET est le début du fichier
        buffer = (char *)malloc(section->sh_size);//alloue la mémoire pour le contenu de la section
        fread(buffer, section->sh_size, 1, fp);//lis le contenu de la section
        if (section->sh_type == SHT_PROGBITS){// si la section est de type PROGBITS
            //on stocke le nom de la section dans nom_section en tant qu'entier
            indice_section=appartient_structure(tab_section_fp,section->sh_name);
            if (indice_section==-1){//si la section n'est pas présente dans tab_section_fp, on l'ajoute
                // on initialise une structure section_elf qui sera ajoutée à tab_section_fp
                section_elf section_temp;
                // on commence par le nom de la section
                section_temp.numero_section = number;
                // si 2 sections ont le même numero mais un nom different : 
                if (index_numero_identique(tab_section_fp,number)!=-1){
                    section_temp.numero_section = nb_elements;
                    nb_elements+=1;
                }
                section_temp.nom_section = section->sh_name;
                // on continue par le contenu de la section (stocké dans buffer)
                section_temp.contenu_section = (char *)malloc(section->sh_size);
                sprintf(section_temp.contenu_section,"%s",buffer);
                memset(chaine,0,sizeof(chaine));

                for (int i = 0; i < section->sh_size; i++){
                    //every 16 bytes, we print a new line
                    if (i % 16 == 0){  
                        strcat(chaine, "\n");
                        sprintf(chaine2, "0x%08x ", section->sh_addr + i);
                        strcat(chaine, chaine2);
                    }
                    //every 8 bytes, we print a space
                    if (i % 4 == 0){
                        sprintf(chaine2," ");
                        strcat(chaine, chaine2);
                    }
                    sprintf(chaine2,"%02x", buffer[i]);
                    strcat(chaine, chaine2);
                }
                // on continue par la taille de la section
                section_temp.taille_section = section->sh_size;
                // on continue par le type de la section
                section_temp.type_section = section->sh_type;
                // on continue par l'offset de la section
                section_temp.offset_section = section->sh_offset;
                // on ajoute l'adresse de la section 
                section_temp.adresse_section = section->sh_addr;
                // on met le contenu de la chaine en tant que contenu de la section 
                section_temp.chaine = malloc(sizeof(chaine));
                strcpy(section_temp.chaine,chaine);
                
                // on ajoute la structure section_elf à tab_section_fp;
                ajout_structure(tab_section_fp,section_temp);
            }
            else{
                // on initialise une structure section_elf qui sera ajoutée à tab_section_fp
                section_elf section_temp;
                // on commence par le nom de la section
                section_temp.nom_section = section->sh_name;
                // on continue par le contenu de la section (stocké dans buffer)
                section_temp.contenu_section = (char *)malloc(section->sh_size+tab_section_fp->tableau_sections[indice_section].taille_section);
                sprintf(section_temp.contenu_section,"%s",buffer);
                memset(chaine,0,10000); // remise à 0 de chaine

                for (int i = 0; i < section->sh_size; i++){
                    //every 16 bytes, we print a new line
                    if (i % 16 == 0){  
                        strcat(chaine, "\n");
                        sprintf(chaine2, "0x%08x ", section->sh_addr + i);
                        strcat(chaine, chaine2);
                    }
                    //every 8 bytes, we print a space
                    if (i % 4 == 0){
                        sprintf(chaine2," ");
                        strcat(chaine, chaine2);
                    }
                    sprintf(chaine2,"%02x", buffer[i]);
                    strcat(chaine, chaine2);
                }
                sprintf(chaine2,"%s", tab_section_fp->tableau_sections[indice_section].chaine);
                strcat(chaine, chaine2);
                // on continue par la taille de la section
                tab_section_fp->tableau_sections[indice_section].taille_section += section->sh_size;
                tab_section_fp->tableau_sections[indice_section].chaine = malloc(sizeof(chaine));
                strcpy(tab_section_fp->tableau_sections[indice_section].chaine,chaine);
            }
        }
    }
    free(buffer);
}

void modif_result(FILE *result, tab_section_elf *tab_section_fp) {
    Elf32_Ehdr header;
    Elf32_Shdr section;
    fread(&header, sizeof(Elf32_Ehdr), 1, result);
    fread(&section, sizeof(Elf32_Shdr), 1, result);
    unsigned char *buffer;

    for(int i = 0; i < header.e_shnum; i++) {
        fseek(result, header.e_shoff + i * header.e_shentsize, SEEK_SET); //déplace le pointeur de fichier au début de la section
        fread(&section, sizeof(Elf32_Shdr), 1, result); //read the section header
        fseek(result, section.sh_offset, SEEK_SET); //déplace le pointeur de fichier au début du contenu de la section. SEEK_SET est le début du fichier
        buffer = (char *)malloc(section.sh_size); //alloue la mémoire pour le contenu de la section
        fread(buffer, section.sh_size, 1, result); //lis le contenu de la section
        if(section.sh_type == SHT_PROGBITS) { // si la section est de type PROGBITS
            section.sh_size = tab_section_fp->tableau_sections[i].taille_section;
            section.sh_offset = tab_section_fp->tableau_sections[i].offset_section;
            section.sh_addr = tab_section_fp->tableau_sections[i].adresse_section;
            section.sh_name = tab_section_fp->tableau_sections[i].nom_section;
            // on écrit le contenu de la section dans le fichier
            fseek(result, section.sh_offset, SEEK_SET); //déplace le pointeur de fichier au début du contenu de la section. SEEK_SET est le début du fichier
            fwrite(tab_section_fp->tableau_sections[i].contenu_section, section.sh_size, 1, result); //lis le contenu de la section
        }
    }
}
