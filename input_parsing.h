#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>

void free_2D(char **data);

int my_strcmp(char *str1, char *str2){
    while (*str1 == *str2) {
	 	if (*(str1 + 1) == 0  &&  *(str2 + 1) == 0) {
	 		return 1;
	 	}
		str1++;
		str2++;
    }
    return 0;
}


char *dyn_read_line(){
    char *buffer = NULL;
    size_t alloc_size = 1;
    size_t size = 0;
    char ch = 0;

    while ((ch = getchar()) != EOF && ch != '\n') {
	if ((size + 1) >= alloc_size) {
		alloc_size *= 2;
	    char *pOld = buffer;
	    buffer = (char*) realloc(buffer, sizeof(char) * (alloc_size + 1));
	    if (!buffer) {
			printf("allocation failled");
			free(pOld);
			return NULL;
 	    }
	} 
	buffer[size++] = ch;
    }
    buffer[size] = 0; //last nul

    return buffer;
}

char *dyn_read_line_FILE(FILE *f){
    
	char *buffer = NULL;
    size_t alloc_size = 1;
    size_t size = 0;
    char ch = 0;


    while ((ch = getc(f)) != EOF) {
        if (ch == ' ') //white space out
        {       
            continue;
        }
        
        if ((size + 1) >= alloc_size) { //jedno misto na \0
            alloc_size *= 2;
            char *pOld = buffer;
            buffer = (char*) realloc(buffer, sizeof(char) * (alloc_size + 1));
            if (!buffer) {
                printf("allocation failled\n");
                free(pOld);
                return NULL;
            }
        } 

        if (ch == '\n') //prerusuju az tady a ne testem ve while, pac potrebuju aby se aspon naalokovala pamet pro \0
        {
            break; // new line nezapisu, prerusim cyklus a na alokovane misto se nize zapise \0
        }
        
        buffer[size++] = ch;
    }

    if (buffer == NULL) { //nic se nealokovalo pac prvni nacteny znak je EOF
		return buffer; //buffer == NULL v tomto pripade => resi znovo volani na soubor, kt uz dosahl EOF
    }
    
    buffer[size] = 0; //last nul
	
    return buffer;
}



char **reading_file(FILE *f){
    size_t alloc_size = 1;
    char **inputBase = NULL;
    char *lineBuffer = NULL; 
    while ((lineBuffer = dyn_read_line_FILE(f))){ 
            char **pOld = inputBase;
            inputBase = (char**)realloc(inputBase, sizeof(char*) * (alloc_size + 1));
            if (!inputBase)
            {
                printf("allocation failled");
                if (alloc_size == 1)
                {
                    // free(pOld); //to je NULL v prvi iteraci stejne nic se tam nealokovaola
                    free(lineBuffer); //jen uvolni s linebuffru co se kvuli selhani nemelo kam ulozit
                    return NULL;
                }
                free(lineBuffer); //uvolnim aktualni alkkovoany radek
                //NULL NEJDRIV MUSIM ULOLZIT NA SPRAVNE MISTO V pOLD !
                pOld[alloc_size - 1] = NULL;
                free_2D(pOld); //necham uvolnit to co se uz zapsalo a je ukonceno NULL !
                return NULL;
                    
            }
            
            inputBase[alloc_size - 1] = lineBuffer;
            alloc_size++;
        }
        inputBase[alloc_size - 1] = NULL;
       
    return inputBase;
}

size_t substring_count(char* str, char delimetr){
    if (!str || !*str)
    {
        return 0;
    }

    size_t count = 0;

    while (*str != '\0')
    {
        if (*str == delimetr)
        {
            count++;
        }
        str++;
    }

    count++; // jeden deli => dva tokeny
    return count;
    
}

char **input_line_parser(char* line, size_t *subsCount, char delimetr){
    if (!line || !*line) 
    {
        return NULL; 
       
    }

    char* wordBuffer = NULL;

    *subsCount = substring_count(line, delimetr); // vyskyt delim + 1 pocet sloupcu vlastne
    char** row = (char**)malloc(sizeof(char*) * (*subsCount + 1)); // +1 pro NULL
    memset(row,0,sizeof(char*)*(*subsCount + 1));

    if (!row){
        printf("allocation failled\n");
        return NULL;
    }

    size_t allocSize = 16;
    wordBuffer = (char*)malloc(sizeof(char) * allocSize);
    memset(wordBuffer, 0, (sizeof(char) * allocSize));

    if (!wordBuffer){
        printf("allocation failled\n");
        return NULL;
    }

    size_t i = 0;
    size_t j = 0;
    while (*line && *subsCount != 0)
    {
        if (j + 1 >= allocSize)
        {
            allocSize *= 2;
            char* pOld =  wordBuffer;
            wordBuffer = (char*)realloc(wordBuffer,sizeof(char) * (allocSize + 1));
            if (!wordBuffer)
            {
                printf("allocation failled\n");
                free(pOld);
                return NULL;
                
            }
            
        }
        

        if (*line == delimetr){
            line++; //posunu se za ',' v 
            wordBuffer[j] = '\0'; //zapisu Nul, melo by pro ni byt vzdy alokly misto
            j = 0; //vynuluju sl
            row[i++] = wordBuffer; //posunu o radek niz
            wordBuffer = (char*)malloc(sizeof(char) * allocSize);
            memset(wordBuffer, 0, (sizeof(char) * allocSize));

            if (!wordBuffer){
                printf("allocation failled\n");
                return NULL;
            }
        }
        else{
            wordBuffer[j++] = *line++;
        }
    }
    
    //zapsani posledni polozky a o radek niz final NULL
    if (wordBuffer)
    {
        row[i++] = wordBuffer;
        wordBuffer = NULL;
        row[i] = NULL;
    }

    return row;
}