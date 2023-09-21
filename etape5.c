#include "projet.h"
#include "print.c"

int main(int argc, char *argv[]){
    FILE *fp;
    if (argc != 2){
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    fp = fopen(argv[1], "rb");
    if (fp == NULL){
        printf("Error: Cannot open file %s\n", argv[1]);
        exit(1);
    }
    
    // Lire les 4 premiers octets du fichier
    FILE * fpcopie= fopen(argv[1], "rb");
    unsigned char buf[4];
    ssize_t n = fread(buf, 1, sizeof(buf), fpcopie);
    if (n < 0 ){
        perror("lecture");
        return 1;
    }
    // Vérification si les octets correspondent à l'en-tête ELF
    if (!(buf[0] == 0x7F && buf[1] == 'E' && buf[2] == 'L' && buf[3] == 'F')){
        printf("Ce n'est pas un fichier ELF.\n");
        return 1;
    }
    fclose(fpcopie);
    print_rel(fp);
    fclose(fp);
    return 0;
}