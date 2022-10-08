/**
 * \file main.c
 * \brief It will check the user input and call all the functions used to compress and decompress a file depending on what the user choose
 * \date 2021
 */

#include "../include/types.h"
#include "../include/macros_constants_headers.h"
#include "../include/file_functions.h"
#include "../include/huffman_coding_table.h"
#include "../include/compression.h"
#include "../include/decompression.h"
#include <time.h>  // Used to get how much time the compression and the decompression take


/**
 * \fn int main(int argc, char** argv)
 * \brief Main function of this Huffman project
 * \param argc An integer that contains the number of arguments given
 * \param argv An array containing the arguments given (it's an array of arrays of characters)
 * \return 0 if the function runs and exits correctly
 */

int main(int argc, char** argv)
{
    TreeNode* huffmanTree = NULL;
    unsigned char * huffmanArray[N_VALUES_IN_BYTE];
    int originalFileSize=0;
    int outputFileSize=0;
    ListNode* listOfNodes=NULL;
    int arrayOfOccurrences[N_VALUES_IN_BYTE];
    FILE* fileInput = NULL;
    FILE* fileOutput = NULL;
    Buffer bufferPos;
    Buffer bufferChar;
    int c_flush=0; // Used to flush stdin
    bufferPos.content=NULL;
    bufferChar.content=NULL;
    unsigned char fileNameInput[FILENAME_MAX];
    unsigned char fileNameOutput[FILENAME_MAX];
    int option=-1; //0: compress, 1: decompress
    clock_t t_start, t_end;

    //DISPLAY THE HELP
    if(argc>1 && !strncmp(argv[1], "-h", 2)){
        printf("\nNAME\n\thuffman\n\nSYNOPSIS\n\thuffman\n\thuffman [OPTION] SOURCE DEST\n\nDESCRIPTION\n\tCompresses or decompresses the file SOURCE by using Huffman coding and saves it in the file DEST.\n\n\t-h\n\t\tdisplay this help and exit.\n\n\t-c\n\t\tcompress SOURCE to DEST.\n\n\t-d\n\t\tdecompress SOURCE to DEST.\n\n");
        return 0;
    }

    //CHECK PARAMETERS
    if(argc==1){ //No parameters
        do{
            printf("\nPress 'c' to compress a file or 'd' to decompress it: ");
            if(EOF==(option=fgetc(stdin))){
                fprintf(stderr, "ERROR: can't get the user choice in main()\n");
                exit(EXIT_FAILURE);
            }
            option-='c'; // if it's 'd' then we have option-='c'=0+1 because 'd' is just after 'c' in the ASCII table
            do{// We flush stdin
                c_flush=getchar();
            }while(c_flush!=EOF && c_flush!='\n');
        }while(option!=0 && option!=1);

        if(option==0){
            printf("\nEnter the name of the file that will be compressed: ");
            getFileName(fileNameInput);
        }
        else{
            printf("\nEnter the name of the file that will be decompressed: ");
            getFileName(fileNameInput);
        }
        
        printf("\nEnter the name of the file in which you want to save the result: ");
        getFileName(fileNameOutput);
    }
    else if(argc==4){ //3 parameters
        if(strlen(argv[1])!=2){
            //Display an error message and recommend to use -h
            fprintf(stderr, "ERROR: bad parameters. Please use the huffman -h for more information\n");
            exit(EXIT_FAILURE);
        }
        else if(!strncmp(argv[1], "-c", 2)){
            strncpy(fileNameInput, argv[2], FILENAME_MAX);
            strncpy(fileNameOutput, argv[3], FILENAME_MAX);
            option=0;
        }
        else if(!strncmp(argv[1], "-d", 2)){
            strncpy(fileNameInput, argv[2], FILENAME_MAX);
            strncpy(fileNameOutput, argv[3], FILENAME_MAX);
            option=1;
        }
        else{
            fprintf(stderr, "ERROR: bad parameters. Please use the huffman -h for more information\n");
            exit(EXIT_FAILURE);
        }
    }
    else{ 
        fprintf(stderr, "ERROR: bad parameters. Please use the huffman -h for more information\n");
        exit(EXIT_FAILURE);
    }

    //COMPRESS
    if(option==0){
        fileInput=fopen(fileNameInput, "rb");
        checkFopen(fileInput);
        t_start=clock();
        printf("Counting the characters...\n");
        originalFileSize=createArrayOfOccurrences(arrayOfOccurrences, fileInput);
        if(originalFileSize==0){
            printf("This file is empty. Please give a file with at least one character\n");
            return 0;
        }
        else if(originalFileSize<0){
            fprintf(stderr, "ERROR: the size of the file wasn't correctly calculated\n");
            exit(EXIT_FAILURE);
        }
        
        printf("Creating the Huffman tree...\n");
        listOfNodes=createListOfNodes(arrayOfOccurrences);
        huffmanTree=createHuffmanTree(&listOfNodes);
        freeList(&listOfNodes);
    
        fileOutput=fopen(fileNameOutput, "wb");
        checkFopen(fileOutput);
        initializeBuffersPosChar(&bufferPos, &bufferChar);
        printf("Saving the tree...\n");
        if(saveHuffmanTree(huffmanTree, &bufferPos, &bufferChar, fileOutput, originalFileSize)){ // There is only one type of characters
            printf("Compressing %s...\n", fileNameInput); 
            freeTree(&huffmanTree);
            t_end=clock();
            printf("Done (%.2f s)\n", ((float)(t_end-t_start))/CLOCKS_PER_SEC);
            outputFileSize=getSizeOfFile(fileOutput);
            printf("%.2f kB compressed to %.2f kB (%.2f %%)",  ((float)originalFileSize)/1000, ((float)outputFileSize)/1000, (((float) outputFileSize)/originalFileSize)*100);
        }
        else{ // There are at least two types of characters
            printf("Preparing the compression...\n");
            createHuffmanArray(huffmanTree, huffmanArray);
            
            freeTree(&huffmanTree);
            printf("Compressing %s...\n", fileNameInput);
            huffManCompression(fileInput, huffmanArray, fileOutput);
            t_end=clock();
            freeArray(huffmanArray);
            printf("Done (%.2f s)\n", ((float)(t_end-t_start))/CLOCKS_PER_SEC);
            outputFileSize=getSizeOfFile(fileOutput);
            printf("%.2f kB compressed to %.2f kB (%.2f %%)",  ((float)originalFileSize)/1000, ((float)outputFileSize)/1000, (((float) outputFileSize)/originalFileSize)*100);
        }
    }
    else if(option==1){
        //DECOMPRESS
        

        fileInput=fopen(fileNameInput, "rb");
        checkFopen(fileInput);
        fileOutput=fopen(fileNameOutput, "wb");
        checkFopen(fileOutput);
        t_start=clock();
        printf("Getting data from the file...\n");
        getDataFromCompressedFile(fileInput, &originalFileSize, &bufferChar, &bufferPos);

        if(originalFileSize<1 || bufferChar.size<1){
            fprintf(stderr, "ERROR: the data read from the file header is incorrect\n");
            exit(EXIT_FAILURE);
        }

        if(bufferPos.size<=0){
            printf("Decompressing %s...\n", fileNameInput);
            for(int i=0; i<originalFileSize; i++)
                fputc(bufferChar.content[0], fileOutput);
            t_end=clock();
            printf("Done (%.2f s)\n", ((float)(t_end-t_start))/CLOCKS_PER_SEC);
        }
        else{
            printf("Building the Huffman tree from data...\n");
            huffmanTree=buildHuffmanTreeFromBuffers(&bufferPos, &bufferChar);

            printf("Decompressing %s...\n", fileNameInput);
            huffManDecompression(fileInput, originalFileSize, huffmanTree, fileOutput);
            t_end=clock();
            printf("Done (%.2f s)\n", ((float)(t_end-t_start))/CLOCKS_PER_SEC);
            freeTree(&huffmanTree);
        }
    }
    else{
        fprintf(stderr, "ERROR: incorrect option value\n");
        exit(EXIT_FAILURE);
    }
    fcloseAndCheck(fileInput);
    fcloseAndCheck(fileOutput);
    free(bufferChar.content);
    free(bufferPos.content);
    
    return 0;
}