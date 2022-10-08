/**
 * \file macros_constants_headers.h
 * \brief Defines all the macros and constants used
 * \date 2021
 */

#ifndef MACROS_CONSTANTS_HEADERS_H
#define MACROS_CONSTANTS_HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Used for strncmp, strchr and strncpy in main.c and file_functions.c

//CONSTANTS

/**
 * \def N_VALUES_IN_BYTE 
 * \brief Constant corresponding to the number of possible values taken by a byte
 */

#define N_VALUES_IN_BYTE 256

//MACROS


/**
 * \def MALLOC(VAR, TYPE, SIZE)
 * \brief Macro used to allocate memory and check if it was correctly done, if not then the program is stopped
 * \param VAR Name of the variable where the memory is being allocated
 * \param TYPE Type of the variable where the memory is being allocated
 * \param SIZE Size of the memory that is being allocated
 */


#define MALLOC(VAR, TYPE, SIZE){\
    VAR=(TYPE*) malloc(sizeof(TYPE)*SIZE);\
    if(VAR==NULL){\
        fprintf(stderr, "ERROR: can't allocate memory\n");\
        exit(EXIT_FAILURE);\
    }\
}

/**
 * \def REALLOC(VAR, TYPE, SIZE)
 * \brief Macro used to reallocate memory and check if it was correctly done, if not then the program is stopped
 * \param VAR Name of the variable where the memory is being reallocated
 * \param TYPE Type of the variable where the memory is being reallocated
 * \param SIZE The new size of the memory that is being reallocated
 */


#define REALLOC(VAR, TYPE, SIZE){\
    VAR=(TYPE*) realloc(VAR, sizeof(TYPE)*SIZE);\
    if(VAR==NULL){\
        fprintf(stderr, "ERROR: can't reallocate memory\n");\
        exit(EXIT_FAILURE);\
    }\
}




#endif