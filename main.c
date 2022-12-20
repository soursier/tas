#include <stdio.h>
#include <string.h>
#include "tas.h"


int main()
{
    init_heap();
    char *p1, *p2, *p3, *p4,*p5,*p6;
    p1 = (char *) tas_malloc(10);
    printf("test \n");
    p2 = (char *) tas_malloc(20);
    printf("test \n");
    p3 = (char *) tas_malloc(20);
    printf("test \n");
    p5 = (char *) tas_malloc(50);
    printf("test \n");
    strcpy( p1, "tp 1" );
    printf("test \n");
    printf("p1 = %s\n",p1);
    strcpy( p2, "tp 2" );
    strcpy( p3, "tp 3" );
    printf("p1 = %s p2 = %s p3 = %s \n",p1,p2,p3);
    tas_free(p2);
    p4 = (char *) tas_malloc(9);
    strcpy( p4, "systeme" );
    strcpy( p5, "tp 5" );
    printf("p4 = %s\n",p4);
    print_heap();
    tas_free(p4);
    tas_free(p1);
    print_heap();
    p6 = (char *) tas_malloc(30);
    strcpy( p6, "tp 6" );
    print_heap();
    return 0;
}