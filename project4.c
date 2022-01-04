#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>

//dej apk do hlavicek inputhandiling, parsingInput, database manipulation


typedef enum {EXIT, ADD_ROW, ADD_COL, AVERAGE, MAX, MIN, SUM} statement;

int determin_command(char *cmd){
    char *cmd_array[] = {"addrow", "addcol", "average", "max", "min", "sum"};
    for (size_t i = 0; i < 6; i++) {
		if (!strcmp(cmd, *(cmd_array + i))) {
			return i;	
		}
    }
    
    return -1;
}

//struct
//item
/*typedef struct{
    node *next;
	node *previous;
	char *row;
} node;

//dynamic List
typedef struct {
    node **data;
}list;*/

//TOHLE MUZEM UDELAT JAKO INPUT STRUKTURU a pak jinou strukturu pro naparsovane slova nebo jak? nebo jak to bude s tim odelovacem nehcam si ho?????
//input struktura je to 2D dynamicke pole
//tohle vymysli jako finale
typedef struct {
	char *entry; //word == column ..to bych moh taky dynamicky alokovat....nejdriv vybuduj dynamicke alokvani struktur 2d <- do char pak ulozim dyn alkovanou pamet se slovem dyn_read() should do
	struct node *next;
}node;

typedef struct {
	node *first;
	node *last;
	size_t nRows;
	size_t nCols;
} list;



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

//tahle neni treba pac zname nejdelsi mozny vstup, ale muzem tim resit zadanou blbost dlouou (ale to
//by se dalo nejaky fscanf nebo neco cemu zadam jen nacet prvnich znaku)
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
    
	//list *myList = NULL;
	
	char *buffer = NULL;
    size_t alloc_size = 1;
    size_t size = 0;
    char ch = 0;


    while ((ch = getc(f)) != EOF) {
        if ((size + 1) >= alloc_size) { //jedno misto na \0
            alloc_size *= 2;
            char *pOld = buffer;
            buffer = (char*) realloc(buffer, sizeof(char) * (alloc_size + 1));
            if (!buffer) {
                printf("allocation failled");
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
	//NEEEEE problem v posledni iteraci pruchodu uz tam zahodi ukazatel s alokovanou pameti a vrati null > bude chybet posledni radek
    /*if (ch = EOF)
	{
		return NULL;
	}*/
	
    if (buffer == NULL) { //nic se nealokovalo pac prvni nacteny znak je EOF
		return buffer; //buffer == NULL v tomto pripade
    }
    
    buffer[size] = 0; //last nul
	
    return buffer;
}

void print_line(char *dataLine){
	for (size_t i = 0; dataLine[i]; i++)
	{
		printf("%c",*(dataLine +i));
	}
	
}


void print_promt(void){
    printf("zadejte prikaz: ");
}

void free_2D(void** data){
    for (size_t i = 0; data[i]; i++) {
	free(data[i]);
	data[i] = NULL;
    }
    free(data);
    data = NULL;
}

int main (int argc, char* argv[]){

	
    
	FILE* f=fopen(argv[1], "r");

	//char *readData = dyn_read_line_FILE(f);
	char *lineBuffer = NULL; 
	char **inputBase = NULL;
	size_t alloc_size = 1;

// pridej nejaky &flag pro EOF a predej ho dynread_FILE tam kdyz poprve doshne EOF tak ho zmeni na
// 1 a cekuj ho ve while jako prvni at uz se pak nevola ta fce co by zapsala za pamet ...flag && read_dyn_FILE....                                                            

	while ((lineBuffer = dyn_read_line_FILE(f))){ //dyn_read_line_FILE nevraci NULL kdyz uz vse nacetlo, jen kdyz selze alokace
	    inputBase = (char**)realloc(inputBase, sizeof(char*) * (alloc_size + 1));
	    inputBase[alloc_size - 1] = lineBuffer;
	    alloc_size++;
	}
	inputBase[alloc_size - 1] = NULL;
	size_t nRows = alloc_size - 1;

	printf("po inputBase\n");
	
	//enquye new_node(readData);
	//
	
//	wVVhile (/* condition */)
//	{
		/* code */
//	}
	
	//char **page = (char**)realloc()

	size_t i = 0;
	while (*(inputBase + i)){
	    print_line(inputBase[i]);
	    i++;
	}
	
	//REPL
    char *command = NULL;
    while (true) {
		print_promt();
		command = dyn_read_line();
	//	get_line();

		if (my_strcmp(command, "exit")){
            free(command);
	    	command = NULL;
    		free_2D((void**)inputBase);
		
        	return EXIT_SUCCESS;
		}


		//compare string against exit
		//else 
		switch (determin_command(command)){
			case 0: 
				//addrow();
				printf("addrow\n");
				continue;
			case 1: 
		//		addcol();
				printf("addcol\n");
				continue;
		
			case 2: 
		//		max();
				printf("average\n");
				continue;
			case 3: 
		//		min();
				printf("max\n");
				continue;
			case 4: 
		//		sum();
				printf("min\n");
				continue;
			case 5: 
		//		sum();
				printf("min\n");
				continue;
			default:
				printf("prikaz nerozpoznan\n");
				continue;
		}
		
		
	//	free(readData);
	//	readData = NULL;
    }
    
    

    return 0;
}
