#include <iostream>
#include <stdio.h>      
#include <stdlib.h>  
#include  "a3_binary_search_tree.hpp"

using namespace std;

BinarySearchTree::Node::Node(DataType newVal){
	left=NULL;
	right=NULL;
	val = newVal;
}
BinarySearchTree::BinarySearchTree() {
	root_=NULL;
	size_=0;
}
BinarySearchTree::~BinarySearchTree() {
	delete root_;
	root_=NULL;
}
void BinarySearchTree::increasesize() {
	size_++;
}
void BinarySearchTree::decreasesize() {
	size_--;
}
unsigned int BinarySearchTree::size() const{
	return size_;
}
BinarySearchTree::DataType BinarySearchTree::max() const{
	Node* a= root_;
	while(a->right != NULL){
		a=a->right;
	}
	return a->val;
}
BinarySearchTree::DataType BinarySearchTree::min() const{
	Node* a= root_;
	while(a->left != NULL){
		a=a->left;
	}
	return a->val;
}
BinarySearchTree::Node* BinarySearchTree::getRootNode(){
	return root_;
}
BinarySearchTree::Node** BinarySearchTree::getRootNodeAddress(){
	return &root_;
}
void BinarySearchTree::setRootNode(Node* kap){
	root_ = kap;
}
bool BinarySearchTree::exists(DataType val) const{
	Node* current=root_;
	bool exists_=true;
	if(size_==0){
		return false;
	}
	while(current->val!=val&&exists_!=false){
		if(val>current->val){
			if(current->right==NULL){
			exists_=false;
			}
			else{
			current=current->right;
			}
		}
		else if(val<current->val){
			if(current->left==NULL){
			exists_=false;
			}
			else{
			current=current->left;
			}
		}
	}
	return exists_;
}
int BinarySearchTree::getNodeDepth(Node* current) const {
	if(current == NULL)
		return -1;
	else	
		return 1 + std::max(getNodeDepth(current->left), getNodeDepth(current->right));
}
unsigned int BinarySearchTree::depth() const{
	return getNodeDepth(root_);
	
}
void BinarySearchTree::updateNodeBalance(Node* current){
	if(current != NULL){
		current->avlBalance = abs(getNodeDepth(current->right)-getNodeDepth(current->left));
	}
	
}
bool BinarySearchTree::insert(DataType val){
	Node* nouveau = new Node(val);
	if(root_==NULL){
		root_= nouveau;
		size_++;
		return true;
	}
	Node* current=root_;
	while(root_!=NULL){	
		if(current->val==val){
			return false;
		 }
		else if(current->val<val){
			if(current->right==NULL){
				current->right= nouveau;
				current->right->val =val;
				size_++;
				return true;
			}
			else{
				current=current->right;
			}
		 }
		else if(current->val>val){
			if(current->left==NULL){
				current->left= nouveau;
				current->left->val =val;
				size_++;
				return true;
			}
			else{
				current=current->left;
			}
		 }
	}
}
BinarySearchTree::Node* BinarySearchTree::search(DataType val, Node** parentnode){
	Node* current=root_;
	*parentnode = root_;
	bool lag=false;
	DataType k=0;
	if(exists(val)){
		while(root_!=NULL){
			if(current->val==val){
				return current;
			}
			else if(current->val<val){
					current=current->right;
					lag=true;
			 }
			else if(current->val>val){
					current=current->left;
					lag=true;
			}
			if(lag&&(k!=0)){
				if((*parentnode)->val > val){
					(*parentnode)=(*parentnode)->left;
				}
				else if((*parentnode)->val<val){
					(*parentnode)=(*parentnode)->right;
				}
			}
			k=1;
		}
	}
}
BinarySearchTree::Node* BinarySearchTree::predecessor_sucessor(Node* track){
		if((track)->right!=NULL){
			(track)=(track)->right;
			while((track)->left!=NULL){
				(track)=(track)->left;
			}
			return (track);
		}
		else{
			(track)=(track)->left;
			while((track)->right!=NULL){
				(track)=(track)->right;
			}
			return (track);	
		}
}
bool BinarySearchTree::remove(DataType val){
	if(size_==0){
		return false;
	}
	if(!(exists(val))){
		return false;
	}
	if(size_==1){
		root_=NULL;
		size_--;
		return true;
	}
	Node* toberemoved; 
	Node* parent;
	Node* cess;
	Node* cess_parent;
	Node* temp;
	Node* temp2;
	toberemoved = search(val,&parent);
	if(toberemoved->left==NULL&&toberemoved->right==NULL){
		if(parent->right == NULL){
			parent->left = NULL;
			delete toberemoved;
			toberemoved = NULL;
			size_--;
			return true;
		}
		else if(parent->left == NULL){
			parent->right = NULL;
			delete toberemoved;
			toberemoved = NULL;
			size_--;
			return true;
		}
		else if(parent->left->val == val){
			parent->left = NULL;
			delete toberemoved;
			toberemoved = NULL;
			size_--;
			return true;
		}
		else if(parent->right->val == val){
			parent->right = NULL;
			delete toberemoved;
			toberemoved = NULL;
			size_--;
			return true;
		}
	}
	else if(toberemoved->left!=NULL&&toberemoved->right==NULL&&(toberemoved->left->right==NULL&&toberemoved->left->left==NULL)){
		toberemoved->val = toberemoved->left->val;
		toberemoved->left =NULL;
		size_--;
		return true; 
	}
	else if(toberemoved->left==NULL&&toberemoved->right!=NULL&&(toberemoved->right->right==NULL&&toberemoved->right->left==NULL)){
		toberemoved->val = toberemoved->right->val;
		toberemoved->right =NULL;
		size_--;
		return true; 
	}
	else{
		cess= predecessor_sucessor(toberemoved);
		temp=search(cess->val,&cess_parent);
		toberemoved->val = cess->val;
		if(cess->right==NULL&&cess->left==NULL){
			if(cess_parent->right->val== cess->val){
				cess_parent->right=NULL;
			}
			else if(cess_parent->left->val== cess->val){
				cess_parent->left=NULL;
			}
		}
		else if(cess->val>root_->val){
			cess_parent->left = cess->right;
		}
		else if(cess->val<root_->val){
			cess_parent->right = cess->left;
		}
		size_--;
		return true;
	}
	
	
}

