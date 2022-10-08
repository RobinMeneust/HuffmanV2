/**
 * \file huffman_coding_table.c
 * \brief Contains functions that create a tree and an array used to compress and decompress a file
 * \date 2021
 */


#include "../include/types.h"
#include "../include/macros_constants_headers.h"
#include "../include/huffman_coding_table.h"

/**
 * \fn void freeTree(TreeNode** tree)
 * \brief Frees the tree that is given
 * \param tree The root of the tree that has to be freed
 */

void freeTree(TreeNode** tree)
{
    if(*tree!=NULL){
        freeTree(&((*tree)->left));
        freeTree(&((*tree)->right));
        free(*tree);
        *tree=NULL;
    }
}


/**
 * \fn void freeList(ListNode** head)
 * \brief Frees the list that is given
 * \param head Head of the list that has to be freed
 */

void freeList(ListNode** head)
{
    ListNode* node=*head;
    ListNode* temp=NULL;
    while(node!=NULL){
        temp=node;
        node=node->next;
        free(temp);
    }
    *head=NULL;
}


/**
 * \fn void copyArray(unsigned char* source, unsigned char* destination, int size)
 * \brief Copies the given array to the destination
 * \param source Copied array
 * \param destination Array where the source is copied
 * \param size Number of elements copied from source to destination. Both arrays must have a size greater or equal to this value
 */

void copyArray(unsigned char* source, unsigned char* destination, int size)
{
    for(int i=0; i<size; i++){
        destination[i]=source[i];
    }
}

/**
 * \fn void freeArray(unsigned char * huffmanArray[N_VALUES_IN_BYTE])
 * \brief Frees the huffman array that is given
 * \param huffmanArray Array that is freed. Its size is fixed and it contains pointers to a dynamically allocated array
 */

void freeArray(unsigned char * huffmanArray[N_VALUES_IN_BYTE])
{
    for(int i=0; i<N_VALUES_IN_BYTE; i++){
        if(huffmanArray[i]!=NULL){
            free(huffmanArray[i]);
            huffmanArray[i]=NULL;
        }
    }
}

/**
 * \fn TreeNode* createTreeNode(int cInput, int occurrenceInput, TreeNode* leftNodeInput, TreeNode* rightNodeInput)
 * \brief Creates a node of a Huffman tree and initializes it by using the given parameters
 * \param cInput Character that the node will be containing
 * \param occurrenceInput Number of occurrences that the node will be containing
 * \param leftNodeInput Pointer to the left node of this newly created node
 * \param rightNodeInput Pointer to the right node of this newly created node
 * \return The new tree node that was created
 */

TreeNode* createTreeNode(int cInput, int occurrenceInput, TreeNode* leftNodeInput, TreeNode* rightNodeInput)
{
    TreeNode* node=NULL;
    MALLOC(node, TreeNode, 1);
    node->c=cInput;
    node->occurrence=occurrenceInput;
    node->left=leftNodeInput;
    node->right=rightNodeInput;
    return node;
}

/**
 * \fn ListNode* createListNode(TreeNode* x, ListNode* nextInput)
 * \brief Creates a list node and initializes it by using the given parameters
 * \param x Element contained by the newly created list node
 * \param nextInput Pointer to the next node of this new node
 * \return The new list node that was created
 */

ListNode* createListNode(TreeNode* x, ListNode* nextInput)
{
    ListNode* node=NULL;
    MALLOC(node, ListNode, 1);
    node->element=x;
    node->next=nextInput;

    return node;
}

/**
 * \fn void push(ListNode** head, TreeNode* x)
 * \brief Adds an element at the beginning of the given list
 * \param head Pointer to the head of the list in which we add an element
 * \param x Pointer to the element that has to be inserted
 */

void push(ListNode** head, TreeNode* x)
{
    ListNode* newListNode=createListNode(x, NULL);
    newListNode->next=*head;
    *head=newListNode;
}


/**
 * \fn TreeNode* popFirst(ListNode** head)
 * \brief Removes the head from the list and returns its content
 * \param head Pointer to the head of the list from which we get the first element
 * \return Pointer to the first node of the list
 */

TreeNode* popFirst(ListNode** head)
{
    TreeNode* returnedElement=NULL;
    ListNode* newHead=NULL;
    if(*head==NULL){
        fprintf(stderr, "ERROR: the list given to popFirst() is empty\n");
        return NULL;
    }
    else{
        returnedElement=(*head)->element;
        newHead=(*head)->next;
        free(*head);
        *head=newHead;
        return returnedElement;
    }
}

/**
 * \fn ListNode* createListOfNodes(int *arrayOfOccurrences)
 * \brief Creates a list of tree nodes by using the number of occurrences of each character
 * \param arrayOfOccurrences Array containing the number of each character given has an index, i.e arrayOfOccurrences['a']=2 means that 'a' appears twice
 * \return The list of tree nodes used to build the Huffman tree
 */

ListNode* createListOfNodes(int *arrayOfOccurrences)
{
    ListNode* returnedList=NULL;
    TreeNode* node=NULL;
    for(int i=0; i<N_VALUES_IN_BYTE; i++)
    {
        if(arrayOfOccurrences[i]!=0){
            node=createTreeNode(i, arrayOfOccurrences[i], NULL, NULL);
            push(&returnedList, node);
        }
    }
    return returnedList;
}

/**
 * \fn void initializeBuffersPosChar(Buffer* bufferPos, Buffer* bufferChar)
 * \brief Initializes bufferPos and bufferChar
 * \param bufferPos Buffer containing all the movements made while saving the tree, to be able to rebuild it
 * \param bufferChar Buffer containing all the characters of the leaves of the tree, sorted in the same order as they are read by this function (following the movements recorded in bufferPos)
 */

void initializeBuffersPosChar(Buffer* bufferPos, Buffer* bufferChar)
{
    bufferPos->size=1000; // it will be increased if it's too small (with a realloc)
    bufferChar->size=N_VALUES_IN_BYTE; //N_VALUES_IN_BYTE is the maximum size here
    MALLOC(bufferPos->content, unsigned char, bufferPos->size);
    MALLOC(bufferChar->content, unsigned char, bufferChar->size);
}

/**
 * \fn int createArrayOfOccurrences(int *arrayOfOccurrences, FILE* fileInput)
 * \brief Creates an array that links each character to its number of occurrences in the file given in parameters
 * \param arrayOfOccurrences Array containing the number of occurrences of the characters in fileInput. To get the value the character is used as an index, i.e arrayOfOccurrences['a']=2 means that 'a' appears twice
 * \param fileInput File from which we get the number of occurrences of each characters
 * \return The size of fileInput
 */

int createArrayOfOccurrences(int *arrayOfOccurrences, FILE* fileInput)
{
    int c=0;
    int fileSize=0;
    for(int i=0; i<N_VALUES_IN_BYTE; i++)
        arrayOfOccurrences[i]=0;

    while((c=fgetc(fileInput))!=EOF){
        arrayOfOccurrences[c]++;
        fileSize++;
    }
    return fileSize;
}

/**
 * \fn ListNode* listMinOccurrence(ListNode* head, ListNode** nodeBeforeMinElement)
 * \brief Gets from the list the node that has the lowest occurrence value and the previous node
 * \param head Pointer to the head of the list from which we get the minimum element
 * \param nodeBeforeMinElement Pointer to the node that is just before the returned element
 * \return Pointer to the node with the lowest occurence value
 */

ListNode* listMinOccurrence(ListNode* head, ListNode** nodeBeforeMinElement)
{
    ListNode* minNode=NULL;
    ListNode* lastNode=NULL;
    ListNode* currentNode=NULL;


    if(head==NULL){
        fprintf(stderr, "ERROR: listMinOccurrence() can't get a node, the list is empty\n");
        return NULL;
    }
    else{
        minNode=head;
        *nodeBeforeMinElement=NULL;
        currentNode=head->next;
        lastNode=head;
        while(currentNode!=NULL){
            if(currentNode->element->occurrence<minNode->element->occurrence){
                minNode=currentNode;
                *nodeBeforeMinElement=lastNode;
            }
            lastNode=currentNode;
            currentNode=currentNode->next;
        }
        return minNode;
    }
}


/**
 * \fn TreeNode* popMin(ListNode** head)
 * \brief Removes from the list the node that has the lowest occurrence value and returns its content
 * \param head Pointer to the head of the list from which we get the minimum element
 * \return Pointer to the tree node with the lowest occurence value in the list
 */

TreeNode* popMin(ListNode** head)
{
    ListNode* minNode=NULL;
    ListNode* nodeBeforeMinNode=NULL;
    ListNode* currentNode=NULL;
    TreeNode* returnedElement;
    if(*head==NULL){
        return NULL;
    }
    else{
        minNode=listMinOccurrence(*head, &nodeBeforeMinNode);
        if(nodeBeforeMinNode==NULL){ // i.e minNode==*head
            return popFirst(head);
        }
        else{
            nodeBeforeMinNode->next=minNode->next;
            returnedElement=minNode->element;
            free(minNode);
            return returnedElement;
        }
    }
}

/**
 * \fn TreeNode* mergeNodes(TreeNode* nodeToMergeLeft, TreeNode* nodeToMergeRight)
 * \brief Merges 2 tree nodes into one by creating a new one that has them as a right and left node
 * \param nodeToMergeLeft Pointer to tree node that will become the left node of the new node
 * \param nodeToMergeRight Pointer to tree node that will become the right node of the new node
 * \return Tree created by merging the 2 given tree nodes
 */

TreeNode* mergeNodes(TreeNode* nodeToMergeLeft, TreeNode* nodeToMergeRight)
{
    TreeNode* mergedNode=NULL;
    mergedNode=createTreeNode(0, 0, nodeToMergeLeft, nodeToMergeRight);
    if(nodeToMergeLeft!=NULL)
        mergedNode->occurrence+=nodeToMergeLeft->occurrence;
    if(nodeToMergeRight!=NULL)
        mergedNode->occurrence+=nodeToMergeRight->occurrence;

    return mergedNode;
}

/**
 * \fn TreeNode* createHuffmanTree(ListNode** head)
 * \brief Creates the Huffman tree from a list of tree nodes
 * \param head Pointer head of the list containing the leaves of the tree
 * \return Huffman tree created from this list
 */

TreeNode* createHuffmanTree(ListNode** head)
{
    TreeNode* nodeToMergeLeft=NULL;
    TreeNode* nodeToMergeRight=NULL;
    TreeNode* mergedNode=NULL;

    if(*head==NULL){
        fprintf(stderr, "ERROR: the list given to createHuffmanTree() is empty\n");
        return NULL;
    }

    if((*head)->next==NULL){ // There is only one node in the list and thus only one character (with one or more occurrences)
        return popFirst(head);
    }
    else{
        while(*head!=NULL){
            nodeToMergeLeft=popMin(head);
            nodeToMergeRight=popMin(head);
            mergedNode=mergeNodes(nodeToMergeLeft, nodeToMergeRight);
            if((*head)!=NULL) // because if there is no element remaining then all the nodes are merged
                push(head, mergedNode);
        }
    }
    return mergedNode;
}

/**
 * \fn void insertInBufferPos(unsigned char *buffer, Buffer *bufferPos, int *i_BufferPos, int *filling)
 * \brief Inserts in bufferPos the filled buffer, reset the values and increase the size of bufferPos if needed
 * \param buffer Byte that will be inserted in bufferPos
 * \param bufferPos Buffer containing all the movements made while saving the tree, to be able to rebuild it
 * \param i_BufferPos Current index of the array contained in bufferPos
 * \param filling Integer that goes from 0 to 8. 0 means empty and 8 means full.
 */

void insertInBufferPos(unsigned char *buffer, Buffer *bufferPos, int *i_BufferPos, int *filling)
{
    bufferPos->content[*i_BufferPos]=*buffer;
    (*i_BufferPos)++;
    *buffer=0;
    *filling=0;
    if(*i_BufferPos>bufferPos->size-3){
        bufferPos->size+=1000;
        REALLOC(bufferPos->content, unsigned char, bufferPos->size);
    }
}



/**
 * \fn void fillBuffers(TreeNode* tree, Buffer *bufferPos, Buffer *bufferChar, int *i_BufferPos, int *i_BufferChar, unsigned char *buffer, int* filling)
 * \brief Fills the buffers bufferPos and bufferChar from the given tree
 * \param tree Tree that is being used to fill the buffers. It needs to have 0 or 2 children, which is always the case with a Huffman tree.
 * \param bufferPos Buffer containing all the movements made while saving the tree, to be able to rebuild it
 * \param bufferChar Buffer containing all the characters of the leaves of the tree, sorted in the same order as they are read by this function (following the movements recorded in bufferPos)
 * \param i_BufferPos Current index of the array contained in bufferPos
 * \param i_BufferChar Current index of the array contained in bufferChar
 * \param buffer Byte that will be inserted in bufferPos
 * \param filling Integer that goes from 0 to 8. 0 means empty and 8 means full.
 */

void fillBuffers(TreeNode* tree, Buffer *bufferPos, Buffer *bufferChar, int *i_BufferPos, int *i_BufferChar, unsigned char *buffer, int* filling)
{
    if(tree->left==NULL && tree->right==NULL){
        bufferChar->content[*i_BufferChar]=tree->c;
        (*i_BufferChar)++;
    }
    else{
        (*buffer)<<=1;
        (*buffer)|=0b1;
        (*filling)++;
        if(*filling>=8)
            insertInBufferPos(buffer, bufferPos, i_BufferPos, filling);
        fillBuffers(tree->left, bufferPos, bufferChar, i_BufferPos, i_BufferChar, buffer, filling);
        (*buffer)<<=1;
        (*buffer)|=0b1;
        (*filling)++;
        if(*filling>=8)
            insertInBufferPos(buffer, bufferPos, i_BufferPos, filling);
        fillBuffers(tree->right, bufferPos, bufferChar, i_BufferPos, i_BufferChar, buffer, filling);
    }
    (*buffer)<<=1;
    (*filling)++;
    if(*filling>=8)
        insertInBufferPos(buffer, bufferPos, i_BufferPos, filling);
}

/**
 * \fn int saveHuffmanTree(TreeNode* tree, Buffer *bufferPos, Buffer *bufferChar, FILE* fileOutput, int fileSize)
 * \brief Saves the given Huffman tree in the compressed file and in 2 buffers
 * \param tree Tree that is being saved. It needs to have 0 or 2 children, which is always the case with a Huffman tree.
 * \param bufferPos Buffer containing all the movements made while saving the tree, to be able to rebuild it
 * \param bufferChar Buffer containing all the characters of the leaves of the tree, sorted in the same order as they are read by this function (following the movements recorded in bufferPos)
 * \param fileOutput Compressed file
 * \param fileSize Size of the original file. It will be saved in the compressed file since it's used to decompress it
 * \return 0 if there is at least 2 different characters and 1 if there is only one
 */

int saveHuffmanTree(TreeNode* tree, Buffer *bufferPos, Buffer *bufferChar, FILE* fileOutput, int fileSize)
{
    int i_BufferChar=0;
    int i_BufferPos=0;
    unsigned char buffer=0;
    int filling=0;
    if(!(tree->left||tree->right)){ // It's a leaf. It means there is only one character in the original file, so it will be a special case
        if(fprintf(fileOutput, "%d\n%d\n%d\n%c", fileSize, 0, 1, tree->c)==EOF){
            fprintf(stderr, "ERROR: fprintf can't write in the output file in saveHuffmanTree\n");
            exit(EXIT_FAILURE);
        }
        return 1;
    }
    else{
        fillBuffers(tree, bufferPos, bufferChar, &i_BufferPos, &i_BufferChar , &buffer, &filling);
        if(filling>0){
            buffer<<=(8-filling);
            bufferPos->content[i_BufferPos]=buffer;
            i_BufferPos++;
        }
        bufferPos->size=i_BufferPos;
        bufferChar->size=i_BufferChar;
        if(fprintf(fileOutput, "%d\n%d\n%d\n", fileSize, bufferPos->size, bufferChar->size)==EOF){
            fprintf(stderr, "ERROR: fprintf can't write in the output file in saveHuffmanTree\n");
            exit(EXIT_FAILURE);
        }
        if(fwrite(bufferPos->content, 1, bufferPos->size, fileOutput)<bufferPos->size){
            fprintf(stderr, "ERROR: fwrite can't write in the output file in saveHuffmanTree\n");
            exit(EXIT_FAILURE);
        }
        if(fputc('\n', fileOutput) == EOF){
            fprintf(stderr, "ERROR: fputc can't write in the output file in saveHuffmanTree\n");
            exit(EXIT_FAILURE);
        }
        if(fwrite(bufferChar->content, 1, bufferChar->size, fileOutput)<bufferChar->size){
            fprintf(stderr, "ERROR: fwrite can't write in the output file in saveHuffmanTree\n");
            exit(EXIT_FAILURE);
        }
        if(fputc('\n', fileOutput) == EOF){
            fprintf(stderr, "ERROR: fputc can't write in the output file in saveHuffmanTree\n");
            exit(EXIT_FAILURE);
        }
        return 0;
    }
}

/**
 * \fn void getDataFromCompressedFile(FILE* fileInput, int* fileSize, Buffer* bufferChar, Buffer* bufferPos)
 * \brief Gets the header contained in the compressed file
 * \param fileInput compressed file from which data is extracted
 * \param fileSize Size of the original file.
 * \param bufferChar Buffer containing all the characters of the leaves of the tree, sorted in the same order as they are read by this function (following the movements recorded in bufferPos)
 * \param bufferPos Buffer containing all the movements made while saving the tree, to be able to rebuild it
 */

void getDataFromCompressedFile(FILE* fileInput, int* fileSize, Buffer* bufferChar, Buffer* bufferPos)
{
    fscanf(fileInput, "%d\n%d\n%d\n", fileSize, &(bufferPos->size), &(bufferChar->size));
    if(bufferPos->size>0){
        MALLOC(bufferPos->content, unsigned char, bufferPos->size);
        if(fread(bufferPos->content, 1, bufferPos->size, fileInput)<bufferPos->size){
            fprintf(stderr, "ERROR: fread can't read the input file in getDataFromCompressedFile\n");
            exit(EXIT_FAILURE);
        }
        fgetc(fileInput); //because there is a '\n' that has to be removed
    }
    MALLOC(bufferChar->content, unsigned char, bufferChar->size);
    if(fread(bufferChar->content, 1, bufferChar->size, fileInput)<bufferChar->size){
        fprintf(stderr, "ERROR: fread can't read the input file in getDataFromCompressedFile\n");
        exit(EXIT_FAILURE);
    }
    fgetc(fileInput); //after this line we are at the beginning of the compressed version of the original file
}

/**
 * \fn void fillAndCheckBufferFromBufferPos(Buffer *bufferPos, int *i_BufferPos, int *nbReadBits, unsigned char *buffer)
 * \brief Resets the buffer and the buffer index, and increases the index of bufferPos
 * \param bufferPos Buffer containing all the movements made while saving the tree, to be able to rebuild it
 * \param i_BufferPos Current index of the array contained in bufferPos
 * \param nbReadBits Number of bits read from the buffer. When it reaches 8, it needs to get the next character of bufferPos in the buffer
 * \param buffer Byte extracted from bufferPos, that is read to move in the tree
 */


void fillAndCheckBufferFromBufferPos(Buffer *bufferPos, int *i_BufferPos, int *nbReadBits, unsigned char *buffer)
{
    *buffer=bufferPos->content[*i_BufferPos];
    (*i_BufferPos)++;
    *nbReadBits=0;
    if(*i_BufferPos>bufferPos->size){
        fprintf(stderr, "ERROR: incorrect bufferPos given to buildHuffmanTreeFromBuffersRec()\n");
    }
}



/**
 * \fn TreeNode* buildHuffmanTreeFromBuffersRec(Buffer *bufferPos, Buffer *bufferChar, int *i_BufferPos, int *i_BufferChar, unsigned char *buffer, int* nbReadBits, unsigned char* prevInstruction)
 * \brief Builds a Huffman tree from the buffers: bufferChar and bufferPos
 * \param bufferPos Buffer containing all the movements made while saving the tree, to be able to rebuild it
 * \param bufferChar Buffer containing all the characters of the leaves of the tree, sorted in the same order as they are read by this function (following the movements recorded in bufferPos)
 * \param i_BufferPos Current index of the array contained in bufferPos
 * \param i_BufferChar Current index of the array contained in bufferChar
 * \param buffer Byte extracted from bufferPos, that is read to move in the tree
 * \param nbReadBits Number of bits read from the buffer. When it reaches 8, it needs to get the next character of bufferPos in the buffer
 * \param prevInstruction Previous instruction that was read from bufferPos
 * \return Tree built from the 2 buffers
 */

TreeNode* buildHuffmanTreeFromBuffersRec(Buffer *bufferPos, Buffer *bufferChar, int *i_BufferPos, int *i_BufferChar, unsigned char *buffer, int* nbReadBits, unsigned char* prevInstruction)
{
    TreeNode* treeNode=createTreeNode(0, 0, NULL, NULL); //the field "occurrence" is no longer used here
    unsigned char instruction=0;
    instruction=((*buffer)>>(7-*nbReadBits))&0b1;
    (*nbReadBits)++;
    if(*nbReadBits>=8)
        fillAndCheckBufferFromBufferPos(bufferPos, i_BufferPos, nbReadBits, buffer);
    if(instruction==0){
        if(*prevInstruction!=0){
            treeNode->c=bufferChar->content[*i_BufferChar];
            (*i_BufferChar)++;

            if(*i_BufferChar>bufferChar->size){
                fprintf(stderr, "ERROR: incorrect bufferChar given to buildHuffmanTreeFromBuffersRec()\n");
            }
            *prevInstruction=0;
        }
    }
    else if(instruction==1){
        if(*prevInstruction==1){
            treeNode->left=buildHuffmanTreeFromBuffersRec(bufferPos, bufferChar, i_BufferPos, i_BufferChar, buffer, nbReadBits, prevInstruction);
        }
        //Here there is *prevInstruction==0

        instruction=((*buffer)>>(7-*nbReadBits))&0b1;
        (*nbReadBits)++;
        if(*nbReadBits>=8)
            fillAndCheckBufferFromBufferPos(bufferPos, i_BufferPos, nbReadBits, buffer);

        if(instruction==1){
            *prevInstruction=1;
            treeNode->right=buildHuffmanTreeFromBuffersRec(bufferPos, bufferChar, i_BufferPos, i_BufferChar, buffer, nbReadBits, prevInstruction);

            instruction=((*buffer)>>(7-*nbReadBits))&0b1;
            (*nbReadBits)++; //return to the parent
            if(*nbReadBits>=8)
                fillAndCheckBufferFromBufferPos(bufferPos, i_BufferPos, nbReadBits, buffer);
            if(instruction!=0){
                fprintf(stderr, "ERROR: incorrect instruction!=0 in bufferPos\n");
                exit(EXIT_FAILURE);
            }
            
        }
        *prevInstruction=0;
    }
    
    return treeNode;
}

/**
 * \fn TreeNode* buildHuffmanTreeFromBuffers(Buffer *bufferPos, Buffer *bufferChar)
 * \brief Builds a Huffman tree from the buffers: bufferChar and bufferPos, by initializing counters to call buildHuffmanTreeFromBuffersRec
 * \param bufferPos Buffer containing all the movements made while saving the tree, to be able to rebuild it
 * \param bufferChar Buffer containing all the characters of the leaves of the tree, sorted in the same order as they are read by this function (following the movements recorded in bufferPos)
 * \return Tree built from the 2 buffers
 */

TreeNode* buildHuffmanTreeFromBuffers(Buffer *bufferPos, Buffer *bufferChar)
{
    int i_BufferPos=1;
    int i_BufferChar=0;
    unsigned char buffer=0;
    int nbReadBits=0;
    unsigned char prevInstruction=1;
    buffer=bufferPos->content[0];

    return buildHuffmanTreeFromBuffersRec(bufferPos, bufferChar, &i_BufferPos, &i_BufferChar, &buffer, &nbReadBits, &prevInstruction);
}



/**
 * \fn void createHuffmanArray(TreeNode* huffmanTree, unsigned char * huffmanArray[N_VALUES_IN_BYTE])
 * \brief Initializes variables sent to createHuffmanArrayRec, and calls it to fill huffmanArray
 * \param huffmanTree Huffman tree used to get the Huffman code for each character
 * \param huffmanArray Array that is being filled in this function. It links all characters to its Huffman code
 */

void createHuffmanArray(TreeNode* huffmanTree, unsigned char * huffmanArray[N_VALUES_IN_BYTE])
{
    unsigned char tempArray[33]; //Array that will be copied in huffmanArray[c] for each character c in the tree.
    // The max length of the code is 33 because the 1st elemnt is used to save the length of the code and the 32 other contains 32 bytes = 256 bits

    int currentByteIndex=1; // index of the current byte in tempArray that is being filled, it must be lesser than 33
    int bitIndex=0;
    for(int i=0; i<N_VALUES_IN_BYTE; i++)
        huffmanArray[i]=NULL; // Initializes the code to NULL

    for(int i=0; i<33; i++)
        tempArray[i]=0; // Initializes buffers in tempArray
    
    createHuffmanArrayRec(huffmanTree, huffmanArray, tempArray, &currentByteIndex, &bitIndex);
}

/**
 * \fn void createHuffmanArrayRec(TreeNode* huffmanTree, unsigned char * huffmanArray[N_VALUES_IN_BYTE], unsigned char tempArray[33], int *currentByteIndex, int *bitIndex)
 * \brief Fills huffmanArray, that will contain all the characters with their Huffman code
 * \param huffmanTree Huffman tree used to get the Huffman code for each character
 * \param huffmanArray Array that is being filled in this function. It links all characters to its Huffman code
 * \param tempArray Array that is being filled by reading the tree and that is (partially) copied in the cell of huffmanArray corresponding to the character in the leaf when we do this copy
 * \param currentByteIndex Index of the current byte in tempArray that is being filled or emptied.
 * \param bitIndex Integer that goes from 0 to 8. 0 means empty and 8 means full. It's the index of the bit edited in the current byte
 */

void createHuffmanArrayRec(TreeNode* huffmanTree, unsigned char * huffmanArray[N_VALUES_IN_BYTE], unsigned char tempArray[33], int *currentByteIndex, int *bitIndex)
{
    unsigned char mask=0; // uses to change a bit
    if(!huffmanTree){
        fprintf(stderr, "ERROR: createHuffmanArrayRec() can't get the Huffman tree\n");
        exit(EXIT_FAILURE);
    }
    if(*bitIndex>=8){
        (*currentByteIndex)++;
        *bitIndex=0;
    }
    if(huffmanTree->left!=NULL){
        mask=0b1;
        mask<<=7-*bitIndex;
        mask=~mask;
        tempArray[*currentByteIndex]&=mask;
        (tempArray[0])++;
        (*bitIndex)++;
        createHuffmanArrayRec(huffmanTree->left, huffmanArray, tempArray, currentByteIndex, bitIndex);
    }
    if(huffmanTree->right!=NULL){
        mask=0b1;
        mask<<=7-*bitIndex;
        tempArray[*currentByteIndex]|=mask;
        (tempArray[0])++;
        (*bitIndex)++;
        createHuffmanArrayRec(huffmanTree->right, huffmanArray, tempArray, currentByteIndex, bitIndex);
    }
    if(huffmanTree->left==NULL && huffmanTree->right==NULL){
        if(huffmanArray[huffmanTree->c]!=NULL){
            fprintf(stderr, "ERROR: Incorrect tree given to createHuffmanArrayRec(). A character appears more than once\n");
            exit(EXIT_FAILURE);
        }
        MALLOC(huffmanArray[huffmanTree->c], unsigned char, ((*currentByteIndex)+1));
        copyArray(tempArray, huffmanArray[huffmanTree->c], (*currentByteIndex)+1);
    }
    //Go to the parent
    (tempArray[0])--;
    (*bitIndex)--;
    if(*bitIndex==-1){
        (*currentByteIndex)--;
        *bitIndex=7;
    }
}