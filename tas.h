/**
 * @file tas.h
 * @author Sours
 * @brief Bibliothèque permettant de gérer une heap
 * @version 0.1
 * @date 2022-11-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TAS_H
#define TAS_H

/**
 * @brief Taille de la heap
 * 
 */
#define SIZE_heap 128

/**
 * @brief Aloue de la mémoire dans la heap
 * 
 * @param taille 
 * @return char* pointeur avec la taille ou NULL si impossible
 */
char* tas_malloc(unsigned int taille);

/**
 * @brief libère de la mémoire le buffer
 * 
 * @param tas 
 */
void tas_free(char* tas);

/**
 * @brief affiche la heap
 * 
 */
void print_heap();

/**
 * @brief Initilialise la heap
 * 
 */
void init_heap();

    #ifdef TEST
    char* get_heap();
    int get_libre();
    void print_heap_debug();
    #endif
#endif
