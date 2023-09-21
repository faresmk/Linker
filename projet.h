#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "elf.h"
#include "libelf.h"
#include "fcntl.h"
#include "unistd.h"
#include "gelf.h"

/******************************************************* Read Fucntions ***************************************************************/

// read  header
Elf32_Ehdr read_header(FILE *fp);

// read section

Elf32_Shdr read_section(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);

//read the symbole table
Elf32_Sym read_symbole(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);
//read the string table (for symbol names)
char *read_string(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);
//read the realocation
Elf32_Rel read_rel(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);
/******************************************************* swtich functions ***************************************************************/

// section

// get section name
char *get_section_name(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);
// get section type
char *get_section_type(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);
// get section flags
char *get_section_flags(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);


// realocations
char *get_relocation_type(Elf32_Word type);


/******************************************************* Print Functions ***************************************************************/

// print header
    void print_header(FILE *fp);

// print section
    void print_section(FILE *fp);

//dispay section content
    void display_section_content(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);
    void print_content_section(FILE *fp);
// Function to read the string table
    char *read_string(FILE *fp, Elf32_Ehdr *header, Elf32_Shdr *section, int i);
//  Function to print the symbole table
    void print_symbole(FILE *fp);

    /****************************************************main.c********************************************************/
//function to call all the phase 1 from the first step to the last
   int phase1(FILE *fp, char choix);




