#include "tas.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>

#include "./linked_list/linked_list.h"
#include "logger.h"

#define CHUNK_LIBRE -1

static char heap[SIZE_heap];
static int libre = 0;
static linked_list head_libre = NULL;

void print_heap()
{
    char* pointer = heap;
    int numero = 0;
    printf("premier octet : %d\n", heap[0]);
    while(pointer-heap<SIZE_heap)
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

void print_heap_debug()
{
    printf ("libre = %d\n", libre);
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 16; j++) { 
            printf("%4d", j + 16*i);
        }
        printf("\n");
        for (j = 0; j < 16; j++) { 
            printf("%4d", heap[j + 16*i]);
        }
        printf("\n");
        for (j = 0; j < 16; j++) { 
            if (isprint(heap[j + 16*i])) {
                printf("%4c", heap[j + 16*i]);
            } else {
                printf("    ");
            }
        }
        printf("\n\n");
    }
    printf("---------------------------------------------------------------\n\n");
}

void init_heap()
{
    heap[0] = SIZE_heap-1;
    heap[1] = CHUNK_LIBRE;
    libre = 0;
    for(int i=2; i<SIZE_heap;i++)
    {
        heap[i]='\0';
    }
    head_libre = linked_list_push(NULL, 0);    
}

char* get_previous_block(char* courant)
{
    char* pointer = &heap[libre];
    int numero = 0;
    char* prec_pointer = pointer;
    do
    {
        numero++;

        if(*(pointer+1)==CHUNK_LIBRE)
        {
            pointer += *pointer+1;
            continue;
        }
        prec_pointer = pointer;
        pointer += *pointer+1;
    }while(pointer+*pointer+1<courant);//la condition aurait dû fonctionner, mais ça prend quand même le courant

    return prec_pointer;
}

char* get_next_block(char* courant)
{
    return courant+*courant+1;
}
void merge_blocks(char* first_block, char* second_block)
{
    *(first_block) += *(second_block)+1; 
}
void merge_free(char* current_block)
{
    char* next_block = get_next_block(current_block);
    if(*(next_block+1)==-1)
    {
        merge_blocks(current_block,next_block);
    }
    if(libre < current_block-heap)
    {
        char* prec_block = get_previous_block(current_block);
        if(*(prec_block+1)==-1)
        {
            merge_blocks(prec_block,current_block);
        }
    }
}

void tas_free(char* tas)
{
    tas[0] = CHUNK_LIBRE;

    int index_libre = linked_list_first_greater_or_equal(head_libre,tas-heap-1);
    if(index_libre==-1)
    {
        linked_list_push(head_libre,tas-heap-1);
        printf("valeur pushée  = %ld\n",tas-heap-1);
    }

    linked_list temp = linked_list_add(head_libre, tas-heap-1, index_libre);
    
    if(index_libre == 0)
    {
        head_libre = temp;
    }

    if(libre > tas-heap )
    {
        libre = tas-heap-1;
    }
    logger_log("FREE", *tas, tas-1);
    char * pointer = tas-1;

    merge_free(pointer);
    linked_list_clean(head_libre);
}

char* first_fit(int taille, char *pred)
{
    linked_list current = head_libre;
    while(current != NULL)
    {
        printf("emplacement = %d valeur de la heap = %d",current->value,heap[current->value]);
        if(heap[current->value] > taille) return &heap[current->value];
        current = current->next;
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
            int index_libre = linked_list_first_greater_or_equal(head_libre,emplacement-heap+taille);
            if(index_libre==-1)
            {
                linked_list_push(head_libre,emplacement-heap+taille);
                printf("valeur pushée  = %ld\n",emplacement-heap+taille);
            }
            linked_list_add(head_libre, emplacement-heap+taille+1, index_libre);
            *(emplacement+taille+1) = *emplacement-taille-1;
            *(emplacement+taille+2) = CHUNK_LIBRE;
        }else
        {
            taille+=1;
        }
    }else
    {
        printf("error !\n");
        printf("taille presente = %d taille demandee = %d",*emplacement,taille);
        return NULL;
    }

    int index_not_libre_anymore = linked_list_first_greater_or_equal(head_libre,emplacement-heap);
    printf("index not libre %d\n",index_not_libre_anymore);
    linked_list temp =  remove_at_index(head_libre,index_not_libre_anymore);
    if((temp!=NULL)&&(temp->before == NULL))
    {
        printf("tempvalue = %d \n",temp->value);
        head_libre = temp;
    }
    *emplacement = taille;
    if(libre==(emplacement-heap))
    {
        libre =(emplacement-heap)+taille+1;
    }
    logger_init_log(LOGGING);
    logger_log("MALLOC", taille, emplacement);
    logger_free();
    printf("taille = %d\n",taille);
    printf("libre = %d\n",libre);
    return emplacement+1;
}

#ifdef TEST
char* get_heap()
{
    return heap;
}
int get_libre()
{
    return libre;
}
#endif  
