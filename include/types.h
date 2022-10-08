/**
 * \file types.h
 * \brief Defines the custom types used
 * \date 2021
 */

#ifndef TYPES_H
#define TYPES_H


/**
 * \struct TreeNode
 * \brief Node of a binary tree containing characters and their occurrences
 */

typedef struct TreeNode{
    unsigned char c; /*!< Character contained in the node. */
    int occurrence; /*!< Number of occurrences of the characters in the leaves of the tree (having this node as a root) */
    struct TreeNode* left; /*!< Pointer to the left node */
    struct TreeNode* right; /*!< Pointer to the right node */
}TreeNode;

/**
 * \struct ListNode
 * \brief Node of a list containing tree nodes
 */

typedef struct ListNode{
    TreeNode* element; /*!< The element contained in a node of the list */
    struct ListNode* next; /*!< Pointer to the next element in the list after this node */
}ListNode;

/**
 * \struct Buffer
 * \brief Buffer containing characters and its size
 */

typedef struct Buffer{
    unsigned char* content; /*!< Contains the characters of the buffer. It has to be dynamically allocated */
    unsigned int size; /*!< Size of the array "content" */
}Buffer;

#endif