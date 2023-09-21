#include "projet.h"
#include "phase2.c"

// phase 1
int phase1(FILE *fp, char choix){   
    FILE *fpcopie = fp;//make a copy of fp
    
    switch (choix){
    case 'h':
        print_header(fpcopie);
        break;
    case 'S':
        print_section(fpcopie);
        break;
    case 'x':
        print_content_section(fpcopie);
    case 's':
        print_symbole(fpcopie);
        break;
    case 'r':
        print_rel(fpcopie);
        break;
    case 'q':
        exit(0);
        break;
    default:
        printf("Choix azfgefzg.\n");
        break;
    }
    fclose(fpcopie);
    return 0;
}

int main(int argc, char *argv[]){
    FILE *fp1;
    FILE *fp2;
    FILE *result;
    if (argc != 3){
        printf("Usage: %s <filename> <filename>\n", argv[0]);
        exit(1);
    }
    fp1 = fopen(argv[1], "rb");
    copie_result(argv[1],"result"); // Copie du fichier fp1 dans result
    result= fopen("result","rb");
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

    char choix;
    char choix2;
    printf("Entrer 1 pour voir les informations du fichier 1\n");
    printf("Entrer 2 pour voir les informations du fichier 2\n");
    printf("Entrer 3 pour voir les informations du fichier résultat\n");
    printf("Entrer q pour quitter\n");
    scanf("%c", &choix);
    scanf("%c", &choix2);
    
    if (choix == '1'){
        printf("Entrée:\n");
        printf("h pour afficher l'en-tête ELF\n");
        printf("S pour afficher les en-têtes de section\n");
        printf("x pour afficher le contenu des sections\n");
        printf("s pour afficher les symboles\n");
        printf("r pour afficher les relocations\n");
        printf("q pour quitter\n");
        scanf("%c", &choix2);
        phase1(fp1, choix2);
    }
    else if(choix == '2'){
        printf("Entrée:\n");
        printf("h pour afficher l'en-tête ELF\n");
        printf("S pour afficher les en-têtes de section\n");
        printf("x pour afficher le contenu des sections\n");
        printf("s pour afficher les symboles\n");
        printf("r pour afficher les relocations\n");
        printf("q pour quitter\n");
        scanf("%c", &choix2);
        phase1(fp2, choix2);
    }
    else if(choix == '3'){
        fpcopie= fopen(argv[1], "rb");
        fpcopie2= fopen(argv[2], "rb");

        Elf32_Ehdr header1;
        Elf32_Shdr section1;
        fread(&header1, sizeof(Elf32_Ehdr), 1, fp1);
        fread(&section1, sizeof(Elf32_Shdr), 1, fp1);
    
        Elf32_Ehdr header2;
        Elf32_Shdr section2;
        fread(&header2, sizeof(Elf32_Ehdr), 1, fp2);
        fread(&section2, sizeof(Elf32_Shdr), 1, fp2);

        tab_section_elf *tab_section_fp = (tab_section_elf *)malloc(sizeof(section_elf)*(header1.e_shnum+header2.e_shnum)+sizeof(int));
        tab_section_fp->taille=0;

        concatene_section_progbits(fp2,&header2,&section2,tab_section_fp);
        concatene_section_progbits(fp1,&header1,&section1,tab_section_fp);

        affiche_structure(tab_section_fp);
        modif_result(result,tab_section_fp); // ici ne marche pas car on ouvre "result" en lecture, on ne peut ouvrir en écriture.
  
        fclose(result);
        fclose(fpcopie);
        fclose(fpcopie2);

    }
    else if (choix == 'q'){
        exit(0);
    }
    else{
        printf("Choix invalide.\n");
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}
