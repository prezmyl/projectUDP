#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>

/*#define ADD_ROW "addrow"
#define ADD_COL "addcol"
#define AVERAGE "average"
#define MAX "max"
#define MIN "min"
#define SUM "sum"
#define EXIT "exit"*/



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

typedef struct {
	char *rowSegmentation;
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
    
	list *myList = NULL;
	
	char *buffer = NULL;
    size_t alloc_size = 1;
    size_t size = 0;
    char ch = 0;

    while ((ch = getc(f)) != EOF /*&& ch != '\n'*/) {
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

void print_line(char *dataLine){
	for (size_t i = 0; dataLine[i]; i++)
	{
		printf("%c",*(dataLine +i));
	}
	
}


void print_promt(void){
    printf("zadejte prikaz: ");
}

int main (int argc, char* argv[]){

	
    
	FILE* f=fopen(argv[1], "r");

	char *readData = dyn_read_line_FILE(f);
//	wVVhile (/* condition */)
//	{
		/* code */
//	}
	
	//char **page = (char**)realloc()

	print_line(readData);

	
	//REPL
    char *command = NULL;
    while (true) {
		print_promt();
		command = dyn_read_line();
	//	get_line();

		if (my_strcmp(command, "exit")){
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
		
		free(command);
		command = NULL;

		free(readData);
		readData = NULL;
    }
    
    

    return 0;
}
