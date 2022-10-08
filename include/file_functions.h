/**
 * \file file_functions.h
 * \brief Contains the functions prototypes of file_functions.c
 * \date 2021
 */

#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

void getFileName(unsigned char fileName[FILENAME_MAX]);
int getSizeOfFile(FILE* file);
void checkFopen(FILE* file);
void fcloseAndCheck(FILE* file);


#endif