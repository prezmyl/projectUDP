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

void free_2D(void** data){
    for (size_t i = 0; data[i]; i++) {
	free(data[i]);
	data[i] = NULL;
    }
    free(data);
    data = NULL;
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
typedef struct entry{
	char **word; //word == column ..to bych moh taky dynamicky alokovat....nejdriv vybuduj dynamicke alokvani struktur 2d <- do char pak ulozim dyn alkovanou pamet se slovem dyn_read() should do
	struct entry *next;
}entry;


typedef struct {
	entry *first;
	entry *last;
	size_t nRows;
	size_t nCols;
} list;


void init_list(list* myList){
    myList->first = NULL;
    myList->last = NULL;
    myList->nCols = 0;
    myList->nRows = 0;
}

//z buffru
void print_line(char *dataLine){
	for (size_t i = 0; dataLine[i]; i++)
	{
		printf("%c",*(dataLine +i));
	}
	
}

void print_2D(char **inputBase){
	size_t i = 0;
	while (*(inputBase + i)){
	    print_line(inputBase[i]);
	    i++;
        printf("\n");
    }

}

void print_entry(list *myList){
    if (!myList)
    {
        return;
    }
    
    entry* current = myList->first;
    while (current != NULL)
    {
        print_2D(current->word);
        current = current->next;
    }

}

//bude pridavat vlastne radky
void add_entry(list* myList, char** parsed_row){
    if (myList == NULL)
    {
        return;
    }
    entry* toInsert=(entry*)malloc(sizeof(entry));
    toInsert->word = parsed_row;
    toInsert->next = NULL;

    if (myList->first == NULL){
        myList->first = toInsert;
        myList->last = toInsert;
    }
    else{
        myList->last->next = toInsert; //nejdriv na posledni entry v listu zmenim jeho next napojim muj
        myList->last = toInsert; //v list zmenim last
    }
    
    (myList->nRows)++; //!!!!!!!!!!! nevim jestli to zvizi cislo nebo co, cekni
 }

//bude slouzit k odstraneni postupne vsech entry seznamu a dealokaci
//pokud by mela slouzit na umazavani radku
void erase_entry(list* myList){
    entry *toDelete = myList->first;
    if (myList->first == NULL)
    {
        //neni tam zadny
        return;
    }
    else
    {   
        //odebirani nejmene predposledniho
        myList->first = toDelete->next;

    }

    //odebrani posledniho
    if (toDelete->next = NULL)
    {
        myList->last = NULL;
    }
    
    free_2D((void**)toDelete->word); //!!!!!!!! free2D predpoklada, ze posleni ukazatel na pole je NULL...delas tak?
    free(toDelete);
    toDelete = NULL;
    
}    


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
	//NEEEEE problem v posledni iteraci pruchodu uz tam zahodi ukazatel s alokovanou pameti a vrati null > bude chybet posledni radek
    /*if (ch = EOF)
	{
		return NULL;
	}*/
	
    if (buffer == NULL) { //nic se nealokovalo pac prvni nacteny znak je EOF
		return buffer; //buffer == NULL v tomto pripade => resi znovo volani na soubor, kt uz dosahl EOF
    }
    
    buffer[size] = 0; //last nul
	
    return buffer;
}




void print_promt(void){
    printf("zadejte prikaz: ");
}





size_t substring_count(char* str){
    if (!str || !*str)
    {
        return 0;
    }

    size_t count = 0;

    while (*str != '\0')
    {
        if (*str == ',')
        {
            count++;
        }
        str++;
    }

    count++; // jeden deli => dva tokeny
    return count;
    
}

char **input_line_parser(char* line, size_t *subsCount){
    if (!line) //prazdny pointer
    {
        return NULL; 
        //mozna prazdny radek by mel vracet prazdny retezec nebo jak s nim bude nalozeno?
    }

    char* wordBuffer = NULL;

   
    printf("input_parser pred printf s\n");
    printf("%s\n",line);
    printf("input_parser po printf s\n\n");
    
    *subsCount = substring_count(line); // vyskyt delim + 1 pocet sloupcu vlastne
    char** row = (char**)malloc(sizeof(char*) * (*subsCount + 1)); // +1 pro NULL

    size_t allocSize = 16;
    wordBuffer = (char*)malloc(sizeof(char) * allocSize);

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
        

        if (*line == ','){
            line++; //posunu se za ',' v 
            wordBuffer[j] = '\0'; //zapisu Nul, melo by pro ni byt vzdy alokly misto
            j = 0; //vynuluju sl
            row[i++] = wordBuffer; //posunu o radek niz
            wordBuffer = (char*)malloc(sizeof(char) * allocSize);
        }
        else{
            wordBuffer[j++] = *line++;
        }
        
        
    }
    
    // !!!JESTE POSLEDNI NULL v nejaky moment tady
    if (wordBuffer)
    {
        row[i++] = wordBuffer;
        row[i] = NULL;
    }

    return row;
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
	    char **pOld = inputBase;
        inputBase = (char**)realloc(inputBase, sizeof(char*) * (alloc_size + 1));
        if (!inputBase)
        {
            printf("allocation failled");
            if (alloc_size == 1)
            {
                // free(pOld); //to je NULL v prvi iteraci stejne nic se tam nealokovaola
                free(lineBuffer); //jen uvolni s linebuffru co se kvuli selhani nemelo kam ulozit
                return 1;
            }
            free(lineBuffer); //uvolnim aktualni alkkovoany radek
            free_2D((void**)pOld); //necham uvolnit to co se uz zapsalo a je ukonceno NULL
            return 1;
                  
        }
        
	    inputBase[alloc_size - 1] = lineBuffer;
	    alloc_size++;
	}
	inputBase[alloc_size - 1] = NULL;
	size_t nRows = alloc_size - 1;

    printf("MAIN pred print_2D input Base \n");
    print_2D(inputBase);
    printf("MAIN po print_2D input Base\n\n");
    
    
    list myList;// = (list*)malloc(sizeof(list)); //!!!dDEALOKUJ nekde
    init_list(&myList);
    
    size_t colsCount = 0; // v tom budu mit pocet sloupcu
    char **buffer;
    size_t actualRow = 0;
    while (inputBase[actualRow])
    {
        //printf("%s",inputBase[actualRow]);
        buffer = input_line_parser(inputBase[actualRow++], &colsCount);
        printf("radek: %d buffer,main pred print_2D input Base \n", actualRow);
        print_2D(buffer);
        printf("buffer,main po print_2D input Base \n\n");
        add_entry(&myList, buffer);
    }


    printf("MAIN pred print_entry \n");
    print_entry(&myList);
    printf("MAIN po print_entry \n");


    
/*
	printf("po inputBase\n");
    
    entry **myEntry = NULL;
    size_t actualRow = 0;
    while (inputBase[actualRow])
    {
        myEntry = (entry**) realloc(myEntry, sizeof(entry*) * (actualRow + 1));
        myEntry[actualRow] = input_line_parser(inputBase[i]);
    }
    
    input_parser();*/

    
	//enquye new_node(readData);
	//
	

	
	//char **page = (char**)realloc()


	


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
