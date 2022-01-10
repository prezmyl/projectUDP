#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>
#include "list_struct.h"
#include "input_parsing.h"


static char *alloc_string(size_t length)
{
    return (char *)malloc(sizeof(char) * (length + 1));
}


void dyn_copy_cmdArgv(char** parsedBuffer, char** dest){

    int i = 0;
    int j = 0;
    int len = 0;

    while (parsedBuffer[i+1]){
        len = strlen(parsedBuffer[i+1]) + 1;
        dest[i] = malloc(sizeof(char) * (len +1));
        j = 0;
        while(parsedBuffer[i+1][j]){
            dest[i][j] = parsedBuffer[i+1][j];
            j++;
        }
        
        dest[i][j] = '\0';
        i++;

    }
    dest[i] = NULL;

}

char *dyn_strcpy(const char *str)
{
    if (!str) {
        return NULL;
    }

    size_t size = strlen(str);

    char *buffer = alloc_string(size);
    if (!buffer) {
        return NULL;
    }

    strncpy(buffer, str, size);
    buffer[size] = '\0';

    return buffer;
}

void free_2D(char **data){
    for (size_t i = 0; data[i]; i++) {
	free(data[i]);
	data[i] = NULL;
    }
    free(data);
    data = NULL;
}


void **dyn_alloc2d(size_t rows, size_t colSizes[rows])
{
    if (rows == 0 || colSizes == NULL) {
        return NULL;
    }

    void **memory = (void **)calloc(rows, sizeof(void *));

    if (memory == NULL) {
        perror("Memory allocation failed!");
        return NULL;
    }

    size_t i = 0;
    for (; i < rows; i++) {
        size_t colSize = colSizes[i];
        if (colSize == 0) {
            continue;
        }
        memory[i] = (void *)malloc(colSize);
        if (memory[i] == NULL) {
            perror("Memory allocation failed!");
            break;
        }
    }

    if (i != rows) {
        free_2D((char**)memory);
    }

    return memory;
}


void exit_seq(list *myList, char* command, char** parsed_command, char** stripped_cmd_argv, char **inputBase,  FILE *f){
      
            free(command);
	    	command = NULL;
            free_2D(parsed_command);
            parsed_command = NULL;
    		
            free(stripped_cmd_argv);

            free_2D(inputBase); //free input 2d buffer
            inputBase = NULL; 
            while (myList->first != NULL) //free all elements in list
            {
                erase_entry(myList);
            }
            fclose(f);   
}


int create_database(list* myList, char **inputBase){
    if (!inputBase || !myList) return 1;

    char **buffer = NULL;
    size_t colsCount = 0;
    size_t actualRow = 0;
    while (inputBase[actualRow])
    {
        buffer = input_line_parser(inputBase[actualRow++], &colsCount, ',');
        add_entry(myList, buffer);
    }
    myList->nCols = colsCount;
    return 0;
}

int determin_command(char *cmd){
    char *cmd_array[] = {"addrow", "addcol", "average", "max", "min", "sum"};
    for (size_t i = 0; i < 6; i++) {
		if (!strcmp(cmd, *(cmd_array + i))) {
			return i;	
		}
    }
    
    return -1;
}

int main (int argc, char* argv[]){

    if (argc != 5)
    {   
        printf("wrong number of parametrs was given..., aborting...\n");
        return 1;
    }
	
    if ((strcmp(argv[1],"-i")))
    {   
        printf("use -i parameter followed by input file..., aborting...\n");
        return 1;
    }
    
    if ((strcmp(argv[3],"-o")))
    {   
        printf("use -o parameter followed by output file..., aborting...\n");
        return 1;
    }

	FILE* f=fopen(argv[2], "r");
    if (!f){
        printf("inccorect file type was given, aborting...\n");
        return 1;
    }

	char **inputBase = NULL;
    inputBase = reading_file(f);
    if (!inputBase)
    {
        printf("reading from the file failed, aborting...\n");
        return 1;
    }
    
    
    list myList;
    init_list(&myList);
    
    
    if (create_database(&myList,inputBase)){
        printf("creation of database failed, wrong input probably...\n");
        return 1;
    }
 

    //print intro
    printf("Soubor: %s, sloupce: %zu, radky: %zu\n", argv[2], myList.nCols, myList.nRows - 1); //-1 ->prvni radek je template sloupcu vzdy


	
    char *command = NULL;
    char **parsed_command = NULL;
    char **stripped_cmd_argv = NULL;
    int argUsedFLAG= 1;
        
    size_t commandArgc = 0;
    
    //REPL
    while (true) {
        //printf("myList.nCols: %zu\n", myList.nCols);
        //printf("myList.nRows: %zu\n", myList.nRows);
      
        if (command)
        {
            free(command);
            command = NULL;
        }
     
        if (parsed_command)
        {
            free_2D(parsed_command);
            parsed_command = NULL;
        }

        if (!argUsedFLAG)
        {
            free_2D(stripped_cmd_argv);
        }
        

		print_promt();
		command = dyn_read_line();
        parsed_command = input_line_parser(command, &commandArgc, ' ');
        stripped_cmd_argv = (char**)malloc(sizeof(char*) * (commandArgc + 2)); 
        memset(stripped_cmd_argv, 0, sizeof(char*) * (commandArgc + 2));
        argUsedFLAG = 0;


		if (my_strcmp(command, "exit")){
            
            FILE *fp;
            fp =fopen(argv[4], "w");
            if (!fp){
                printf("output file was not created right, aborting...\n");
                exit_seq(&myList, command, parsed_command, stripped_cmd_argv, inputBase, f);
                return 1;
            }

         
            fprint_to_file(fp,&myList);

            fclose(fp);
            
            exit_seq(&myList, command, parsed_command, stripped_cmd_argv, inputBase, f);
		
        	return EXIT_SUCCESS;
		}


		switch (determin_command(parsed_command[0])){
			case 0:
                if (commandArgc - 1 != myList.nCols)
                {
                    printf("zadan spatny pocet sloupcu v radku\n");
                    continue;
                }

                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);               
                add_entry(&myList, stripped_cmd_argv);
                argUsedFLAG = 1;
             
				printf("Radek byl pridan\n");
				continue;
			case 1: 
                if (commandArgc - 2 != myList.nRows - 1) 
                {
                    printf("zadan pocet sloupcu neodpovida poctu sloupcu v databazi \n");
                    continue;
                }
                
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                add_col(&myList,stripped_cmd_argv);
              	argUsedFLAG = 0;

                printf("Sloupec byl pridan\n");
				continue;
		
			case 2:
                if (commandArgc != 2) 
                {
                    printf("zadano moc argumentu, za avarage jeden <nazev sloupce> \n");
                    continue;
                }
                
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                double average = 0;
                if ((count_average(&myList,stripped_cmd_argv[0],&average) > 0)){
                    printf("Sloupec neexistuje\n");
                    continue;
                }

				printf("Prumer sloupce %s: %.2f\n", stripped_cmd_argv[0], average);
				continue;
			case 3: 
                if (commandArgc != 2)  
                {
                    printf("zadano moc argumentu, za max jeden <nazev sloupce> \n");
                    continue;
                }
                
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                double max = 0;
                if ((find_max(&myList,stripped_cmd_argv[0],&max) > 0)){
                    printf("Sloupec neexistuje\n");
                    continue;
                }

				printf("Nejvetsi hodnota sloupce %s: %.2f\n", stripped_cmd_argv[0], max);
				continue;
			case 4: 
                if (commandArgc != 2)  
                {
                    printf("zadano moc argumentu, za max jeden <nazev sloupce> \n");
                    continue;
                }
                
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                double min = 0;
                if ((find_min(&myList,stripped_cmd_argv[0],&min) > 0)){
                    printf("Sloupec neexistuje\n");
                    continue;
                }

				printf("Nejmensi hodnota sloupce %s: %.2f\n", stripped_cmd_argv[0], min);
				continue;
			case 5: 
             if (commandArgc != 2)  //-1 jen za addcol, nazev je soucast sloupcu legitimni vzdy v prvnim radky..-1 za prvni radek s nazvy sloupcu
                {
                    printf("zadano moc argumentu, za sum jeden <nazev sloupce> \n");
                    continue;
                }
                
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                double sum = 0;
                if ((count_sum(&myList,stripped_cmd_argv[0],&sum) > 0)){
                    printf("Sloupec neexistuje\n");
                    continue;
                }

				printf("Soucet sloupce %s: %.2f\n", stripped_cmd_argv[0], sum);
				continue;
			default:
				printf("prikaz nerozpoznan\n");
				continue;
		}
		
    }
    
    return 0;
}