/**
 * \file decompression.c
 * \brief Contains functions used to decompress a file by using Huffman coding
 * \date 2021
 */

#include "../include/types.h"
#include "../include/macros_constants_headers.h"
#include "../include/huffman_coding_table.h"
#include "../include/decompression.h"

/**
 * \fn void huffManDecompression(FILE* fileInput, int fileSize, TreeNode* treeHead, FILE* fileOutput)
 * \brief Decompresses a file compressed by using Huffman
 * \param fileInput Compressed file that we want to decompress
 * \param fileSize Number of characters that the decompressed file will contain
 * \param treeHead The head of the Huffman tree that is needed to decompress the file
 * \param fileOutput File where is written the decompressed version of the source file
 */

void huffManDecompression(FILE* fileInput, int fileSize, TreeNode* treeHead, FILE* fileOutput){
    int bit_Position = 7;
    int nbr_insert_char = 0;
    unsigned char c = fgetc(fileInput); //It will get the first byte of fileInput    
    TreeNode* tree_travel = treeHead;

    while(fileSize > nbr_insert_char){
        if((c >> bit_Position)&1){             
            tree_travel = tree_travel->right;
        } 
        else{
            tree_travel = tree_travel->left;
        }
        if(!(tree_travel->left || tree_travel->right)){ // If "tree_travel" is a leaf
            if(fputc(tree_travel->c, fileOutput) == EOF){ //
                fprintf(stderr, "ERROR: fputc can't write in the output file in huffManDecompression\n");
                exit(EXIT_FAILURE);
            }
            nbr_insert_char++;     // we have one more char inserted in the buffer.
            tree_travel = treeHead;        // we start again at the root of the tree in order to run it again for the next char.
        }
        if(bit_Position>0){   // If we're not at the lowest order bit, we just have to read the next one
            bit_Position--;
        } 
        else{
            if(feof(fileInput) && fileSize > nbr_insert_char){  // That means that we have finished reading all the characters of fileInput but we still haven't written all the characters, so it's an error
                fprintf(stderr, "ERROR: the size of the input file isn't correct");
                exit(EXIT_FAILURE);
            }
            bit_Position = 7;    // We reset bit_Position (so that it's on the most significant bit of the new Byte).
            c = fgetc(fileInput);  // We get the next byte from fileInput
        }
    }
}


