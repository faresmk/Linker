#include "projet.h"
#include "read_rel.c"


    // Iterate through the sections of the file and group sections of type PROGBITS with the same name. For each group of sections, concatenate the data of these sections into a new result file.

    // For sections that are present only in one file, simply add these sections to the result file.

    // Determine the concatenation offsets of the sections in the second file by comparing the position of the sections in the result file to their position in the second original file.





//How to merge section type PROGBITS
void concatinate_PROGBITS_sections(FILE *fp1, FILE *fp2)
{
    Elf32_Ehdr header1 = read_header(fp1);
    Elf32_Ehdr header2 = read_header(fp2);
    Elf32_Shdr *section1 = malloc(sizeof(Elf32_Shdr) * header1.e_shnum);
    Elf32_Shdr *section2 = malloc(sizeof(Elf32_Shdr) * header2.e_shnum);
    Elf32_Shdr *section3 = malloc(sizeof(Elf32_Shdr) );
    for (int i = 0; i < header1.e_shnum; i++)
    {
        section1[i] = read_section(fp1, &header1, section1, i);
    }
    for (int i = 0; i < header2.e_shnum; i++)
    {
        section2[i] = read_section(fp2, &header2, section2, i);
    }
    int compteur = 0;
    for (int i = 0; i < header1.e_shnum; i++)
    {
        if (section1[i].sh_type != SHT_PROGBITS)
        {   section3 = realloc(section3, sizeof(Elf32_Shdr) * (compteur + 1));
            section3[compteur] = section1[i];
            compteur++;
           // printf("1\n");
        }
        else 
        {
            for (int j = 0; j < header2.e_shnum; j++)
            {
                if (section2[j].sh_type == SHT_PROGBITS)
                {
                    if (strcmp(get_section_name(fp1, &header1, section1, i), get_section_name(fp2, &header2, section2, j)) == 0)
                    {
                        section3 = realloc(section3, sizeof(Elf32_Shdr) * (compteur + 1));
                        section3[compteur] = section1[i];
                        section3[compteur].sh_size = section1[i].sh_size + section2[j].sh_size;
                        compteur++;
                       // printf("2\n");
                    }
                }
                else 
                {
                    int bool = 1;
                    for (int k = 0; k < header1.e_shnum; k++)
                    {
                        if (strcmp(get_section_name(fp1, &header1, section1, k), get_section_name(fp2, &header2, section2, j)) == 0)
                        {
                            bool = 0;
                        }
                    }
                    if (bool == 1)
                    {
                        section3 = realloc(section3, sizeof(Elf32_Shdr) * (compteur + 1));
                        section3[compteur] = section2[j];
                        compteur++;
                    }
                }
            }
        }
    }

    // Determine the concatenation offsets
    int offset = 0;
    for (int i = 0; i < compteur; i++)
    {
        section3[i].sh_offset = offset;
        offset += section3[i].sh_size;
    }

   Elf32_Ehdr *header3 = calloc(1, sizeof(Elf32_Ehdr));
   header3->e_shstrndx = compteur;
    
    printf("  [Nr] Nom               Type              Adr      Décala Taille ES Fan LN Inf Al \n");
    for (int i = 0; i < header3->e_shstrndx; i++)
    {
         printf("  [%2d] ", i);
        printf("%-18d ", section3[i].sh_name);
        printf("%-18d ", section3[i].sh_type);
        printf("%08x ",  section3[i].sh_addr); 
        printf("%06x ",  section3[i].sh_offset);
        printf("%06x ",  section3[i].sh_size);
        printf("%02x ",  section3[i].sh_entsize);
        printf("%3d ",   section3[i].sh_flags);
        printf("%2d ",   section3[i].sh_link);
        printf("%3d ",   section3[i].sh_info);
        printf("%2d ",   section3[i].sh_addralign);
        printf("\n");
    }
    int i;
    printf("Pour afficher le contenu d'une section, taper un nombre entre 0 et %d\n", header1.e_shnum - 1);
    scanf("%d", &i);
    if (i >= 0 && i < header1.e_shnum)
    {
        display_section_content(fp1,header3, section3, i);
    }else
    {
        printf("Erreur de saisie");
    }
}



// main: read ELF header and print it
int main(int argc, char *argv[]){
    FILE *fp1;
    FILE *fp2;
    if (argc != 3){
        printf("Usage: %s <filename> <filename>\n", argv[0]);
        exit(1);
    }
    fp1 = fopen(argv[1], "rb");
    fp2 = fopen(argv[2], "rb");
    if (fp1 == NULL){
        printf("Error: Cannot open file %s\n", argv[1]);
        exit(1);
    }
    if (fp2 == NULL){
        printf("Error: Cannot open file %s\n", argv[2]);
        exit(1);
    }
    // Lire les 4 premiers octets du fichier
    FILE * fpcopie= fopen(argv[1], "rb");
    FILE * fpcopie2= fopen(argv[2], "rb");
    unsigned char buf[4];
    unsigned char buf2[4];
    ssize_t n = fread(buf, 1, sizeof(buf), fpcopie);
    ssize_t n2 = fread(buf2, 1, sizeof(buf2), fpcopie2);
    if (n < 0 || n2 < 0){
        perror("lecture");
        return 1;
    }
    // Vérification si les octets correspondent à l'en-tête ELF
    if (!(buf[0] == 0x7F && buf[1] == 'E' && buf[2] == 'L' && buf[3] == 'F')){
        printf("Ce n'est pas un fichier ELF.\n");
        return 1;
    }
    if (!(buf2[0] == 0x7F && buf2[1] == 'E' && buf2[2] == 'L' && buf2[3] == 'F')){
        printf("Ce n'est pas un fichier ELF.\n");
        return 1;
    }
    fclose(fpcopie);
    fclose(fpcopie2);
    
    concatinate_PROGBITS_sections(fp1, fp2);

    fclose(fp1);
    fclose(fp2);
    return 0;
}


/*
 if (section1[i].sh_type == SHT_PROGBITS)
        {
            for (int j = 0; j < header2.e_shnum; j++)
            {
                if (section2[j].sh_type == SHT_PROGBITS)
                {
                    if (strcmp(get_section_name(fp1, &header1, section1, i), get_section_name(fp2, &header2, section2, j)) == 0)
                    {
                        section1[i].sh_size = section1[i].sh_size + section2[j].sh_size;
                        section1[i].sh_offset = section1[i].sh_offset + section2[j].sh_offset;
                        section1[i].sh_addralign = section1[i].sh_addralign + section2[j].sh_addralign;
                        section1[i].sh_entsize = section1[i].sh_entsize + section2[j].sh_entsize;
                        section1[i].sh_link = section1[i].sh_link + section2[j].sh_link;
                        section1[i].sh_info = section1[i].sh_info + section2[j].sh_info;
                        section1[i].sh_flags = section1[i].sh_flags + section2[j].sh_flags;
                        section1[i].sh_addr = section1[i].sh_addr + section2[j].sh_addr;
                        section1[i].sh_name = section1[i].sh_name + section2[j].sh_name;
                        section1[i].sh_type = section1[i].sh_type + section2[j].sh_type;
                    }
                }
            }
}*/