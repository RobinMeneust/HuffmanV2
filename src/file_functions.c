/**
 * \file file_functions.c
 * \brief Contains functions used to open and close a file, and to get its name and its size
 * \date 2021
 */

#include "../include/macros_constants_headers.h"
#include "../include/file_functions.h"

/**
 * \fn void getFileName(unsigned char fileName[FILENAME_MAX])
 * \brief Gets the name of a file
 * \param fileName Array of characters that will contain the name of the file to open
 */

void getFileName(unsigned char fileName[FILENAME_MAX])
{
    char *posEndOfInput=NULL;
    if(!fgets(fileName, FILENAME_MAX, stdin)){
        fprintf(stderr, "ERROR: getFileName() can't get the file name\n");
    }
    //Remove the \n (if it exists) that was added when the user typed "Enter"
    posEndOfInput=strchr(fileName, '\n');
    if(posEndOfInput)
        *posEndOfInput='\0';
}

/**
 * \fn int getSizeOfFile(FILE* file)
 * \brief Gives the size of a file
 * \param file File whose size has to be determined
 * \return Size of the file: number of bytes that it contains
 */

int getSizeOfFile(FILE* file)
{
    rewind(file);
    fseek(file, 0, SEEK_END);
    int size = (int) ftell(file);
    rewind(file);
    return size;
}

/**
 * \fn void checkFopen(FILE* file)
 * \brief Checks if a file was opened correctly, if not then the program is stopped
 * \param file File that is tested
 */

void checkFopen(FILE* file)
{
    if(file==NULL){
        fprintf(stderr, "ERROR: the file can't be opened\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * \fn void fcloseAndCheck(FILE* file)
 * \brief Closes a file and checks if it was done properly, if not then the program is stopped
 * \param file File that is closed
 */

void fcloseAndCheck(FILE* file)
{
    if(fclose(file)==EOF){
        fprintf(stderr, "ERROR: the file can't be closed\n");
        exit(EXIT_FAILURE);
    }
}

