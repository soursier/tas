/**
 * @file tas.h
 * @author Sours
 * @brief Bibliothèque permettant de gérer une stack
 * @version 0.1
 * @date 2022-11-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TAS_H
#define TAS_H

/**
 * @brief Taille de la stack
 * 
 */
#define SIZE_STACK 128

/**
 * @brief Aloue de la mémoire dans la stack
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
 * @brief affiche la stack
 * 
 */
void print_heap();

/**
 * @brief Initilialise la stack
 * 
 */
void init_heap();

    #ifdef TEST
    char* get_heap();
    int get_libre();
    void print_stack_debug();
    #endif
#endif
