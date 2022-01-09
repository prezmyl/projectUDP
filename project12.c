#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>
#include "list_struct.h"
//dej apk do hlavicek inputhandiling, parsingInput, database manipulation

static char *alloc_string(size_t length)
{
    return (char *)malloc(sizeof(char) * (length + 1));
}

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

void dyn_copy_cmdArgv(char** parsedBuffer, char** dest){

    int i = 0;
    int j = 0;
    int len = 0;

    printf("\ncoppied parsed\n");
    print_2D(parsedBuffer);
    printf("coppied parsed\n\n");

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
    printf("\ncoppied\n");
    print_2D(dest);
    printf("coppied\n");
}

void free_2D(char **data){
    for (size_t i = 0; data[i]; i++) {
	free(data[i]);
	data[i] = NULL;
    }
    free(data);
    data = NULL;
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
//by se dalo nejaky fscanf nebo neco cemu zadam jen nacet prvnich znaku), hovno parametru muze byt libovolne pokud odpovida poctu radku
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
    if (!line || !*line) //prazdny pointer
    {
        return NULL; 
        //mozna prazdny radek by mel vracet prazdny retezec nebo jak s nim bude nalozeno?
    }

    char* wordBuffer = NULL;

   
   /* printf("input_parser pred printf s\n");
    printf("%s\n",line);
    printf("input_parser po printf s\n\n");*/
    
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

void exit_seq(list *myList, char* command, char** parsed_command, char** stripped_cmd_argv, char **inputBase,  FILE *f){
      
      //VYPROPAGUJOU SE TY NULL, ASI NE MUSEL BYCH PREDAVAT O RAD HLOUBS POINTRY
            free(command);
	    	command = NULL;
            free_2D(parsed_command);
            parsed_command = NULL;
    		
            //tohle resi jen primy exit, jinak po kazdem replu je nastavena na NULL, takze to projde i tady
            free(stripped_cmd_argv);

            free_2D(inputBase); //free input 2d buffer
            inputBase = NULL; 
            while (myList->first != NULL) //free all elements in list
            {
                erase_entry(myList);
            }
            fclose(f);   
}

char **reading_file(FILE *f){
    size_t alloc_size = 1;
    char **inputBase = NULL;
    char *lineBuffer = NULL; 
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
                    return NULL;
                }
                free(lineBuffer); //uvolnim aktualni alkkovoany radek
                //NULL NEJDRIV MUSIM ULOLZIT NA SPRAVNE MISTO V pOLD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                pOld[alloc_size - 1] = NULL;
                free_2D(pOld); //necham uvolnit to co se uz zapsalo a je ukonceno NULL !!!!!!!!!!!!!!!!ja tam jeste NEMAMMMMAMMM NULL VOLE
                return NULL;
                    
            }
            
            inputBase[alloc_size - 1] = lineBuffer;
            alloc_size++;
        }
        inputBase[alloc_size - 1] = NULL;
        //nRows = alloc_size - 1;
    return inputBase;
}


int create_database(list* myList, char **inputBase){
    if (!inputBase || !myList) return 1;

    char **buffer = NULL;
    size_t colsCount = 0;
    size_t actualRow = 0;
    while (inputBase[actualRow])
    {
        //printf("%s",inputBase[actualRow]);
        buffer = input_line_parser(inputBase[actualRow++], &colsCount, ',');
        /*printf("radek: %d buffer,main pred print_2D input Base \n", actualRow);
        print_2D(buffer);
        printf("buffer,main po print_2D input Base \n\n");*/
        add_entry(myList, buffer);
    }
    myList->nCols = colsCount;
    return 0;
}



int main (int argc, char* argv[]){
	
    if ((strcmp(argv[1],"-i")))
    {   
        printf("use -i parameter followed by input file..., aborting...\n");
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
    


    printf("MAIN pred print_2D input Base \n");
    print_2D(inputBase);
    printf("MAIN po print_2D input Base\n\n");
    
    
    list myList;// = (list*)malloc(sizeof(list)); //!!!dDEALOKUJ nekde => je na stacku uvoni se sam po konci programu
    init_list(&myList);
    
    
    if (create_database(&myList,inputBase)){
        printf("creation of database failed, wrong input probably...\n");
        return 1;
    }
    
    //printf("MAIN pred print_entry \n");
    putchar('\n');
    print_entry(&myList);
    putchar('\n');
    //printf("MAIN po print_entry \n");

    //print intro
    printf("Soubor: %s, sloupce: %zu, radky: %zu\n", argv[2], myList.nCols, myList.nRows); //prvni radek je template sloupcu vzdy


	//REPL
    char *command = NULL;
    char **parsed_command = NULL;
    char **stripped_cmd_argv = NULL;
    int argUsedFLAG= 1;
        
    size_t commandArgc = 0;
    
    while (true) {
        printf("myList.nCols: %zu\n", myList.nCols);
        printf("myList.nRows: %zu\n", myList.nRows);
        //ty ukazatele co nikam nepredali alokovanou pamet musim a chci do nich znova zapisovat musim uvolnit vynulovat
        if (command)
        {
            free(command);
            command = NULL;
        }
        //tady bych si to nuloval, kdybych poslal parser 2d pole do fce, ukazovaly by na stejnou adresu aspon nachvili
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
        stripped_cmd_argv = (char**)malloc(/*stripped_cmd_argv,*/ sizeof(char*) * (commandArgc + 2)); //argc ukazuje plus jedna pac prvni radek nekopiruju, takze pro null uz mam misto
        memset(stripped_cmd_argv, 0, sizeof(char*) * (commandArgc + 2));
        argUsedFLAG = 0;

        print_2D(parsed_command);
        printf("commandArgc: %zu\n",commandArgc);


		if (my_strcmp(command, "exit")){
            
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

                printf("adding row\n");
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);               
                add_entry(&myList, stripped_cmd_argv);
                argUsedFLAG = 1;
             
				printf("Radek byl pridan\n");
                putchar('\n');
                print_entry(&myList);
                putchar('\n');
				continue;
			case 1: 
                if (commandArgc - 2 != myList.nRows - 1)  //-1 jen za addcol, nazev je soucast sloupcu legitimni vzdy v prvnim radky..-1 za prvni radek s nazvy sloupcu
                {
                    printf("zadan pocet sloupcu neodpovida poctu sloupcu v databazi \n");
                    continue;
                }
                
				printf("adding col\n");
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                print_2D(stripped_cmd_argv);

                add_col(&myList,stripped_cmd_argv);
              	argUsedFLAG = 0;

                printf("Sloupec byl pridan\n");
                putchar('\n');
                print_entry(&myList);
                putchar('\n');
				continue;
		
			case 2:
                if (commandArgc != 2)  //-1 jen za addcol, nazev je soucast sloupcu legitimni vzdy v prvnim radky..-1 za prvni radek s nazvy sloupcu
                {
                    printf("zadano moc argumentu, za avarage jeden <nazev sloupce> \n");
                    continue;
                }
                
				printf("counting avr\n");
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                print_2D(stripped_cmd_argv); 
                double average = 0;
                if ((count_average(&myList,stripped_cmd_argv[0],&average) > 0)){
                    printf("Sloupec neexistuje\n");
                    continue;
                }

				printf("Prumer sloupce %s: %.2f\n", stripped_cmd_argv[0], average);
				continue;
			case 3: 
                if (commandArgc != 2)  //-1 jen za addcol, nazev je soucast sloupcu legitimni vzdy v prvnim radky..-1 za prvni radek s nazvy sloupcu
                {
                    printf("zadano moc argumentu, za max jeden <nazev sloupce> \n");
                    continue;
                }
                
				printf("counting max\n");
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                print_2D(stripped_cmd_argv); 
                double max = 0;
                if ((find_max(&myList,stripped_cmd_argv[0],&max) > 0)){
                    printf("Sloupec neexistuje\n");
                    continue;
                }

				printf("Nejvetsi hodnota sloupce %s: %.2f\n", stripped_cmd_argv[0], max);
				continue;
			case 4: 
                if (commandArgc != 2)  //-1 jen za addcol, nazev je soucast sloupcu legitimni vzdy v prvnim radky..-1 za prvni radek s nazvy sloupcu
                {
                    printf("zadano moc argumentu, za max jeden <nazev sloupce> \n");
                    continue;
                }
                
				printf("counting min\n");
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                print_2D(stripped_cmd_argv); 
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
                
				printf("counting sum\n");
                dyn_copy_cmdArgv((char**)parsed_command, stripped_cmd_argv);   
                print_2D(stripped_cmd_argv); 
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