#include "a3_binary_search_tree.hpp"
#include <iostream>
#include <cmath>

BinarySearchTree::Node::Node(DataType newVal) {
	val = newVal;
	left = NULL;
	right = NULL;
}

unsigned int BinarySearchTree::size() const {
	return size_;
}

BinarySearchTree::BinarySearchTree() {
	size_ = 0;
	root_ = NULL;
}

BinarySearchTree::DataType BinarySearchTree::max() const {
	Node* current = root_;
	while(current->right != NULL)
		current = current->right;
		
	return current->val;
}

BinarySearchTree::DataType BinarySearchTree::min() const {
	Node* current = root_;
	while(current->left != NULL)
		current = current->left;
		
	return current->val;
}

int BinarySearchTree::getNodeDepth(Node* node) const {
	if(node == NULL)
		return -1;
	return 1 + std::max(getNodeDepth(node->left), getNodeDepth(node->right));
}

unsigned int BinarySearchTree::depth() const {
	return getNodeDepth(root_);
}

void BinarySearchTree::print() const {
	
}

BinarySearchTree::Node* BinarySearchTree::getRootNode() {
	return root_;
}

BinarySearchTree::Node** BinarySearchTree::getRootNodeAddress() {
	return &root_;
}

bool BinarySearchTree::exists(DataType val) const {
	Node *current = root_; 
	
	while(current != NULL) {
		if(current->val == val)
			return true;
		if(val > current->val)
			current = current->right;
		else
			current = current->left;
	}
	
	return false;
}

bool insert_help(BinarySearchTree::Node **T,  BinarySearchTree::Node *N){
	if (*T == NULL) {
		*T = N;
		return true;
	}
	
	if ((*T)->val == N->val)
		return false;
	
	if (N->val > (*T)->val)
		return insert_help(&(*T)->right, N);
	else 
		return insert_help(&(*T)->left, N);
	
}

bool BinarySearchTree::insert(DataType val){
	Node *N = new Node(val);
	
	bool status = insert_help(&root_, N);
	
	if(status)
		size_++;
    
    updateNodeBalance(root_);
    
	return status;
}

void removeNode(BinarySearchTree::Node** root, BinarySearchTree::Node** tbd, BinarySearchTree::Node** tbdParent, bool isLeftChild) {
    if((*tbd)->left == NULL && (*tbd)->right == NULL) {
        delete *tbd;
        if(*root == *tbd) {
            *root = NULL;
            *tbd = NULL;
        }
        else if(isLeftChild)
            (*tbdParent)->left = NULL;
        else
            (*tbdParent)->right = NULL;
        return;
    }
    else if((*tbd)->left != NULL && (*tbd)->right != NULL) {
        BinarySearchTree::Node* successor = (*tbd)->right;
        BinarySearchTree::Node* successorPrev = (*tbd);
        while(successor->left != NULL) {
            successorPrev = successor;
            successor = successor->left;
        }
        
        int temp = successor->val;
        successor->val = (*tbd)->val;
        (*tbd)->val = temp;
        removeNode(root, &successor, &successorPrev, successorPrev != *tbd);
    }
    else if((*tbd)->right == NULL) {
        int temp = (*tbd)->left->val;
        (*tbd)->left->val = (*tbd)->val;
        (*tbd)->val = temp;
        removeNode(root, &(*tbd)->left, tbd, true);
    }
    else {
        int temp = (*tbd)->right->val;
        (*tbd)->right->val = (*tbd)->val;
        (*tbd)->val = temp;
        removeNode(root, &(*tbd)->right, tbd, false);
    }
}

bool BinarySearchTree::remove(DataType val) { 
    Node* current = root_;
    Node* prev = NULL;
    bool isLeftChild = false;
    
    while(current != NULL && current->val != val) {
        if(current->val < val) {
            isLeftChild = false;
            prev = current;
            current = current->right;
        }
        else {
            isLeftChild = true;
            prev = current;
            current = current->left;
        }
    }
    
    if(current == NULL)
        return false;
    
    removeNode(&root_, &current, &prev, isLeftChild);
    size_--;
    updateNodeBalance(root_);
    return true;
}

void BinarySearchTree::updateNodeBalance(Node* n){
    if(n != NULL)
        n->avlBalance = getNodeDepth(n->left) - getNodeDepth(n->right);
}

void destruct(BinarySearchTree::Node *N) {
	if(N != NULL){
		destruct(N->left); 
		destruct(N->right);
		delete N;
	}
}

BinarySearchTree::~BinarySearchTree() { 
	destruct(root_);
} 

