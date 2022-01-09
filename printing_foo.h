#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


void print_promt(void){
    printf("zadejte prikaz: ");
}



//z buffru
void print_line(char *dataLine){
    if (!dataLine) return;
	for (size_t i = 0; dataLine[i]; i++){
		printf("%c",*(dataLine +i));
	}
	
}

void print_2D(char **inputBase){
    if (!inputBase){
        return;
    }
    
	size_t i = 0;
	while (*(inputBase + i)){
	    print_line(inputBase[i]);
	    i++;
        putchar(' ');
    }
    printf("\n\n");
}

