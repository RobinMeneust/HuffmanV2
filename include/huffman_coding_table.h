/**
 * \file huffman_coding_table.h
 * \brief Contains the functions prototypes of huffman_coding_table.c
 * \date 2021
 */

#ifndef HUFFMAN_CODING_TABLE_H
#define HUFFMAN_CODING_TABLE_H


void freeTree(TreeNode** tree);
void copyArray(unsigned char* source, unsigned char* destination, int size);
void freeArray(unsigned char * huffmanArray[N_VALUES_IN_BYTE]);
void freeList(ListNode** head);
TreeNode* createTreeNode(int cInput, int occurrenceInput, TreeNode* leftNodeInput, TreeNode* rightNodeInput);
ListNode* createListNode(TreeNode* x, ListNode* nextInput);
void push(ListNode** head, TreeNode* x);
TreeNode* popFirst(ListNode** head);
void initializeBuffersPosChar(Buffer* bufferPos, Buffer* bufferChar);
ListNode* createListOfNodes(int *arrayOfOccurrences);
int createArrayOfOccurrences(int *arrayOfOccurrences, FILE* fileInput);
ListNode* listMinOccurrence(ListNode* head, ListNode** nodeBeforeMinElement);
TreeNode* popMin(ListNode** head);
TreeNode* mergeNodes(TreeNode* nodeToMergeLeft, TreeNode* nodeToMergeRight);
TreeNode* createHuffmanTree(ListNode** head);
void insertInBufferPos(unsigned char *buffer, Buffer *bufferPos, int *i_BufferPos, int *filling);
void fillBuffers(TreeNode* tree, Buffer *bufferPos, Buffer *bufferChar, int *i_BufferPos, int *i_BufferChar, unsigned char *buffer, int* filling);
int saveHuffmanTree(TreeNode* tree, Buffer *bufferPos, Buffer *bufferChar, FILE* fileOutput, int fileSize);
void getDataFromCompressedFile(FILE* fileInput, int* fileSize, Buffer* bufferChar, Buffer* bufferPos);
void fillAndCheckBufferFromBufferPos(Buffer *bufferPos, int *i_BufferPos, int *nbReadBits, unsigned char *buffer);
TreeNode* buildHuffmanTreeFromBuffersRec(Buffer *bufferPos, Buffer *bufferChar, int *i_BufferPos, int *i_BufferChar, unsigned char *buffer, int* nbReadBits, unsigned char* prevInstruction);
TreeNode* buildHuffmanTreeFromBuffers(Buffer *bufferPos, Buffer *bufferChar);
void createHuffmanArray(TreeNode* huffmanTree, unsigned char * huffmanArray[N_VALUES_IN_BYTE]);
void createHuffmanArrayRec(TreeNode* huffmanTree, unsigned char * huffmanArray[N_VALUES_IN_BYTE], unsigned char tempArray[33], int *currentByteIndex, int *bitIndex);



#endif