#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>
#include "printing_foo.h"

void free_2D(char **data);

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

void add_col(list* myList, char **parsedCols){
    if (myList == NULL) return;

    size_t len = 0;
    size_t nCols = myList->nCols;
    size_t i = 0;
    size_t j = 0;
    entry* current = myList->first;
    while (current != NULL)
    {
        current->word = realloc(current->word, sizeof(char*) * nCols + 2); //!!!!!!!!!
        len = strlen(parsedCols[i]);
        current->word[nCols + 1] = malloc(sizeof(len + 1));
        while (parsedCols[i][j])
        {
            current->word[nCols + 1][j] = parsedCols[i][j];
            j++;
        }
        current->word[nCols + 1][j] = 0;
        current->word[nCols + 2] = NULL;
        current = current->next;
        i++;
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

    printf("\npocet radku: %zu\n", --(myList->nRows));

    free_2D(toDelete->word); //!!!!!!!! free2D predpoklada, ze posleni ukazatel na pole je NULL...delas tak?
    free(toDelete);
    toDelete = NULL;
}    


