#ifndef A3_AVL_TREE_HPP
#define A3_AVL_TREE_HPP

#include "a3_binary_search_tree.hpp"

class AVLTree: public BinarySearchTree
{
public:
	// Overriden insert and remove functions 
	// Do not modify these definitions
    bool insert(DataType val);
    bool remove(DataType val);
    void balanceAVLTree(Node** root);
    void updateNodeBalancetree(Node** root);

	// Define additional functions and attributes that you need below
};
#endif

