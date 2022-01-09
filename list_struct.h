#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>
#include "printing_foo.h"

void free_2D(char **data);
void **dyn_alloc2d(size_t rows, size_t colSizes[rows]);
char *dyn_strcpy(const char *str);

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

void add_col(list* myList, /*entry* current ,*/char **stripped_cmd_argv){
    
    entry* current =myList->first;
    size_t i = 0;
    while (current != NULL)
    {
        printf("pred realoc\n");
        print_2D(current->word);
        current->word = (char**)realloc(current->word, sizeof(char*) * (myList->nCols + 2));  
        current->word[myList->nCols] = dyn_strcpy(stripped_cmd_argv[i]); ///tady dyn strcpy a pointer ulozit a pak uvolnit cely strepped..pac jinak cast pointru bude ulozena do elementu a ukazovat na to misto
        // printf("po prekopirovani slova: %s v %d-tem radku\n", current->word[myList.nCols + 1], i);
        current->word[myList->nCols + 1] = NULL; //ted pole alloc -1 je max oproti malloc(alloc)
        printf("po realoc\n");
        print_2D(current->word);

        current = current->next;
        i++;
    }
    (myList->nCols)++;
}

/*void add_col(list* myList, char **parsedCols){
    if (myList == NULL) return;

        size_t len = 0;
        size_t nCols = myList->nCols;
        printf("add_col() nCols: %zu\n", nCols);
        size_t i = 0;
        size_t j = 0;
        entry* current = myList->first;
        while (current != NULL){
            dyn_alloc2d(nCols + 2,)
        {

}*/

/*void add_col(list* myList, char **parsedCols){
    if (myList == NULL) return;

    size_t len = 0;
    size_t nCols = myList->nCols;
    printf("add_col() nCols: %zu\n", nCols);
    size_t i = 0;
    size_t j = 0;
    entry* current = myList->first;
    while (current != NULL)
    {
        printf("parsed[%d]%s\n", i,parsedCols[i]);
        printf("pred realoc\n");
        print_2D(current->word);
        *(current)->word = realloc(current->word, sizeof(char*) * nCols + 2); //!!!!!!!!!
        printf("po realoc\n");
        print_2D(current->word);
        len = strlen(parsedCols[i]);
        current->word[nCols + 1] = malloc(sizeof(char)*(len + 1));
        j = 0;
        while (parsedCols[i][j])
        {
            current->word[nCols][j] = parsedCols[i][j];
            j++;
        }
        current->word[nCols + 1][j] = 0;
        printf("po prekopirovani slova: %s v %d-tem radku\n", current->word[nCols], i);
        print_2D(current->word);
        current->word[nCols + 2] = NULL;
        printf("addcol() one entry updated\n");
        print_2D(current->word);
       
        current = current->next;
        i++;
    }
    (myList->nCols)++;
    printf("add_col() nCols: %zu\n", myList->nCols);
    
}*/

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
    
     printf("\npocet radku: %zu\n", ++(myList->nRows));; //!!!!!!!!!!! nevim jestli to zvizi cislo nebo co, cekni
 }

//bude slouzit k odstraneni postupne vsech entry seznamu a dealokaci
//pokud by mela slouzit na umazavani radku
void erase_entry(list* myList){
    entry *toDelete = myList->first;
    if (myList->first == NULL){
        //neni tam zadny
        return;
    }
    else{
        //zkopiruju ukazatel
        myList->first = toDelete->next;
    }

    //odebrani posledniho
    if (toDelete->next = NULL)
    {
        myList->last = NULL;
    }

   

    free_2D(toDelete->word); //!!!!!!!! free2D predpoklada, ze posleni ukazatel na pole je NULL...delas tak?
    free(toDelete);
    toDelete = NULL;

    printf("\npocet radku: %zu\n", --(myList->nRows));
}    


