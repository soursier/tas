
#include "tas.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>

#define CHUNK_LIBRE -1

static char stack[SIZE_STACK];
static int libre = 0;
void print_stack()
{
    char* pointer = stack;
    int numero = 0;
    printf("premier octet : %d\n", stack[0]);
    while(pointer-stack<SIZE_STACK)
    {
        numero++;
        if(*(pointer+1)==CHUNK_LIBRE)
        {
            printf("Le bloc n°%d est vide et comprend %d octects a l'adresse %p \n",numero,*pointer,(void*)pointer);
            pointer += *pointer+1;
            continue;
        }
        printf("le bloc n°%d a reserve %d octets a l'emplacement %p : %s \n",numero,*pointer,(void*)pointer,pointer+1);
        pointer += *pointer+1;
    }
}

void print_stack_debug()
{
    printf ("libre = %d\n", libre);
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 16; j++) { 
            printf("%4d", j + 16*i);
        }
        printf("\n");
        for (j = 0; j < 16; j++) { 
            printf("%4d", stack[j + 16*i]);
        }
        printf("\n");
        for (j = 0; j < 16; j++) { 
            if (isprint(stack[j + 16*i])) {
                printf("%4c", stack[j + 16*i]);
            } else {
                printf("    ");
            }
        }
        printf("\n\n");
    }
    printf("---------------------------------------------------------------\n\n");
}

void init_stack()
{
    stack[0] = SIZE_STACK-1;
    stack[1] = CHUNK_LIBRE;
    libre = 0;
    for(int i=2; i<SIZE_STACK;i++)
    {
        stack[i]='\0';
    }
}

char* get_previous_block(char* courant)
{
    char* pointer = &stack[libre];
    int numero = 0;
    char* prec_pointer = pointer;
    printf("premier octet : %d\n", stack[0]);
    do
    {
        numero++;

        if(*(pointer+1)==CHUNK_LIBRE)
        {
            //printf("Le bloc n°%d est vide et comprend %d octects a l'adresse %p \n",numero,*pointer,(void*)pointer);
            pointer += *pointer+1;
            continue;
        }
       // printf("le bloc n°%d a reserve %d octets a l'emplacement %p : %s \n",numero,*pointer,(void*)pointer,pointer+1);
        prec_pointer = pointer;
        pointer += *pointer+1;
    }while(pointer+*pointer+1<courant);//la condition aurait dû fonctionner, mais ça prend quand même le courant

    printf("Le bloc précédent n°%d a pour premier octect %d comprend %d octects a l'adresse %p \n",numero,*(pointer+1),*pointer,(void*)pointer);
    return prec_pointer;
}
char* get_next_block(char* courant)
{
    printf("le suivant = %ld\n",(courant+*courant+1)-stack);
    return courant+*courant+1;
}
void merge_blocks(char* first_block, char* second_block)
{
    printf("le premier block avait %d octets alloués \n",*(first_block));
    printf("le deuxieme block avait %d octets alloués \n",*(second_block));
    *(first_block) += *(second_block)+1; 
    printf("le premier block a %d octets alloués \n",*(first_block));
}
void merge_free(char* current_block)
{
    printf("le courant = %ld\n",(current_block)-stack);
    printf("let's merge\n");

    char* next_block = get_next_block(current_block);
    if(*(next_block+1)==-1)
    {
        printf("next block is free \n");
        merge_blocks(current_block,next_block);
    }
    if(libre < current_block-stack)
    {
        char* prec_block = get_previous_block(current_block);
        printf("le precedant = %ld\n",(prec_block)-stack);
        if(*(prec_block+1)==-1)
        {
            merge_blocks(prec_block,current_block);
            printf("previous block is free\n");
        }
    }
    

}

void tas_free(char* tas)
{
    tas[0] = CHUNK_LIBRE;
    if(libre > tas-stack )
    {
        libre = tas-stack-1;
    }
    char * pointer = tas-1;

    printf("libre = %d, tas-stack = %ld tas-1 = %ld \n",libre,tas-stack,pointer-stack);
    merge_free(pointer);
}

char* first_fit(int taille, char *pred)
{
    char* pointer = &(stack[libre]);
    int numero = 0;
    while(pointer-stack<SIZE_STACK)
    {

        if(*(pointer+1)==CHUNK_LIBRE)
        {
            if(*pointer > taille) return pointer;
            pointer += *pointer+1;
            continue;
        }
        pointer += *pointer+1;
        numero++;
    }
    return NULL;
}

char *tas_malloc(unsigned int taille)
{
    char* emplacement = first_fit(taille,NULL);
    if(emplacement==NULL) return NULL;
    if(taille<*emplacement)
    {
        if(*emplacement-taille>2)
        {
            *(emplacement+taille+1) = *emplacement-taille-1;
            *(emplacement+taille+2) = CHUNK_LIBRE;
            printf("assez grand !\n");
        }else
        {
            printf("petit !\n");
            taille+=1;
        }
    }else
    {
        printf("error !\n");
        printf("taille presente = %d taille demandee = %d",*emplacement,taille);
        return NULL;
    }
    *emplacement = taille;
    if(libre==(emplacement-stack))
    {
        libre =(emplacement-stack)+taille+1;
    }
    printf("taille = %d\n",taille);
    printf("libre = %d\n",libre);
    return emplacement+1;

}

#ifdef TEST
char* get_stack()
{
    return stack;
}
int get_libre()
{
    return libre;
}
#endif