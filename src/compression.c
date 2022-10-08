/**
 * \file compression.c
 * \brief Contains functions used to compress a file by using Huffman coding
 * \date 2021
 */

#include "../include/types.h"
#include "../include/macros_constants_headers.h"
#include "../include/huffman_coding_table.h"
#include "../include/compression.h"

/**
 * \fn void huffManCompression(FILE* fileInput, unsigned char * huffmanArray[N_VALUES_IN_BYTE], FILE* fileOutput)
 * \brief Compresses a file by using Huffman
 * \param fileInput File that is being compressed
 * \param huffmanArray Array that is being filled in this function. It links all the characters to their Huffman code
 * \param fileOutput File where is written the decompressed version of fileInput
 */

void huffManCompression(FILE* fileInput, unsigned char * huffmanArray[N_VALUES_IN_BYTE], FILE* fileOutput)
{
    unsigned char buffer=0; // "character" that will be inserted in fileOutput
    int c=0;
    int size=0;
    int currentByteIndex=1; // current index of huffmanArray[c] for the current character c. 0 corresponds to the index of the cell containing the size
    int currentBitIndex=0; // current bit read in huffmanArray[c][currentByteIndex]. It's between 0 & 8
    int bufferIndex=0; // counts the number of bits inserted in buffer, when it's equal to 8, the buffer is inserted in fileOutput. It's between 0 & 8
    int indexTotalBitsInCode=1; // integer between 0 and size-1 that counts the number of bits read from the code in the huffman array
    rewind(fileInput);
    while(!feof(fileInput)){
        if(indexTotalBitsInCode>=size){ // all bits from huffmanArray[c] has been read or the loop has just started
            c=fgetc(fileInput);
            if(c==EOF) // if we are at the end of the file then we stop
                break;
            size=huffmanArray[c][0];
            if(size==0){
                fprintf(stderr, "ERROR: in huffmanCompression the input file and the huffman array are not compatible\n");
                exit(EXIT_FAILURE);
            }
            currentByteIndex=1;
            currentBitIndex=0;
            indexTotalBitsInCode=0;
        }
        if(c==EOF) // we stop the loop
            break;
        buffer=(buffer<<1)|((huffmanArray[c][currentByteIndex]>>(7-currentBitIndex))&0b1);
        indexTotalBitsInCode++;
        bufferIndex++;
        currentBitIndex++;
    
        if(bufferIndex>=8){ // buffer is full
            if(fputc(buffer, fileOutput)==EOF){
                fprintf(stderr, "ERROR: fputc can't write in the output file in huffmanCompression\n");
                exit(EXIT_FAILURE);
            }
            bufferIndex=0;
            buffer=0;
        }
        if(currentBitIndex>=8){ // we read huffmanArray[c][currentByteIndex] so we will read the next cell (currentByteIndex+1)
            currentByteIndex++;
            currentBitIndex=0;
        }
    }
    if(bufferIndex>0){ // buffer is not empty, we have to add the last buffer to the file
        if(fputc(buffer<<(8-bufferIndex), fileOutput)==EOF){
            fprintf(stderr, "ERROR: fputc can't write in the output file in huffmanCompression\n");
            exit(EXIT_FAILURE);
        }
    }
}