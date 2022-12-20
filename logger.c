#include "logger.h"

#include <stdio.h>
#include <time.h>  

#define MAX_SIZE_DATE 100


static FILE * logs = NULL;
static logger_state_t state = NOT_INITIALIZED;

int logger_init_log(logger_state_t mode)
{
    if(state != NOT_INITIALIZED)
    {
        perror("Logger already running\n");
        return -1;
    }
    if(mode == LOGGING)
    {
        logs = fopen("./logs", "a");
        state = LOGGING;
    }else {
        logs = fopen("./logs", "r");
        state = READING;
    }
    if(logs == NULL)
    {
        perror("can't open logs !\n");
        return -1;
    }
    return 0;
}


int logger_free()
{
    if(logs!=NULL)
        fclose(logs);
    state = NOT_INITIALIZED;
    return 0;
}

void get_time_str(char* time_s)
{
    time_t timestamp = time( NULL );
    struct tm * pTime = localtime( & timestamp );
    strftime(time_s,MAX_SIZE_DATE,"[%B %e %Y][%T]",pTime);
}


int logger_log(char* operation, int size_byte,void* address){
    if(state != LOGGING)
        return -1;
    char time_s[MAX_SIZE_DATE];
    get_time_str(time_s);
    fprintf(logs,"%s %s %d bytes at address %p\n",time_s,operation,size_byte,address);
    if(logs == NULL)
    {
        printf("???\n");
    }
    return 0;
}

int logger_read()
{
    if(state != READING)
        return -1;
    
    int nb_bytes = 0;
    void* address = NULL;
    while(fscanf(logs,"%*s %*s %*s %*s %d bytes at address %p \n",&nb_bytes,&address) != EOF)
    {
        printf("%p %d bytes\n",address,nb_bytes);
    }   
    return 0;
}
