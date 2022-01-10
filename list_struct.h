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


void fprint_to_file(FILE* f, list* myList){
    if (!myList)
    {
        return;
    }
    
    entry* current = myList->first;
    while (current != NULL)
    {
        size_t i = 0;
	    while (current->word[i]){
            
           
		    //fprintf(f,"%c",*(current->word +i));
	         fprintf(f,current->word[i]);   

        i++;
        if (current->word[i] != NULL){
            fputc(',',f);
            fputc(' ',f);
        }
    }
    fputc('\n',f);
    current = current->next;
    }
}

/*void print_to_file( FILE* f, list *myList){
    if (!myList)
    {
        return;
    }
    
    entry* current = myList->first;
    while (current != NULL)
    {
        fprintf(f, "anus\n");
        //print_2D(current->word);
        current = current->next;
    }

}*/

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

int col_match(list* myList, char *str){
    if (!myList || !str || !*str)
    {
        return -1;
    }
    //jen ve first hlavicce hledam
    int i = 0;
    entry* current = myList->first; 
    while (current->word[i])
    {
        if (!strcmp(current->word[i], str))
        {
            return i;
        }
        
        i++;
    }
    return -1;
}

int count_average(list* myList, char *str, double *avr){
    if (!myList || !str || !*str)
    {
        return 1;
    }

    int i = 0;
    if ((i = col_match(myList, str)) < 0){
        return 2;
    }
  
    double sum = 0;
    double count = myList->nRows;
    double num = 0;

    entry* current = myList->first->next; //preskakuji prvni, pac v prvnim entry elementu je hlavicka
    while (current != NULL)
    {
        num = atof(current->word[i]);
        sum += num;
        current = current->next;
    }

    *avr = sum / (myList->nRows-1); //nRows je tam zapocitan i prvni radek s hlavickou sloupcu
    return 0;
}

int count_sum(list* myList, char *str, double *sum){
    if (!myList || !str || !*str)
    {
        return 1;
    }

    int i = 0;
    if ((i = col_match(myList, str)) < 0){
        return 2;
    }
  
    
    double count = myList->nRows;
    double num = 0;

    entry* current = myList->first->next;
    while (current != NULL)
    {
        num = atof(current->word[i]);
        *sum += num;
        current = current->next;
    }

    return 0;
}

int find_max(list* myList, char *str, double *max){
    if (!myList || !str || !*str)
    {
        return 1;
    }

    int i = 0;
    if ((i = col_match(myList, str)) < 0){
        return 2;
    }
    printf("i =%d\n",i);
    
    double count = myList->nRows;
    double num = 0;

    entry* current = myList->first->next;
    *max = atof(current->word[i]);
    while (current != NULL)
    {   
        printf("pro current[%d] je slovo %s\n", i, current->word[i]);
        num = atof(current->word[i]);
        printf("num = %f\n",num);
        if (num > *max){
            *max = num;
        }
        current = current->next;
    }

    return 0;
}

int find_min(list* myList, char *str, double *min){
    if (!myList || !str || !*str)
    {
        return 1;
    }

    int i = 0;
    if ((i = col_match(myList, str)) < 0){
        return 2;
    }
    printf("i =%d\n",i);
    
    
    double count = myList->nRows;
    double num = 0;

    entry* current = myList->first->next;
            
    *min = atof(current->word[i]);
    printf("min= %f\n",*min);
    printf("%f string: %s", *min, current->word[0]);
    while (current != NULL)
    {
        printf("pro current[%d] je slovo %s\n", i, current->word[i]);
        num = atof(current->word[i]);
        printf("num = %f\n",num);
        if (num < *min){
            *min = num;
            printf("min= %f\n",*min);
        }
        current = current->next;
    }

    return 0;
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

   

    free_2D(toDelete->word); //!!!!!!!! free2D predpoklada, ze posleni ukazatel na pole je NULL...delas tak?
    free(toDelete);
    toDelete = NULL;

    printf("\npocet radku: %zu\n", --(myList->nRows));
}    


