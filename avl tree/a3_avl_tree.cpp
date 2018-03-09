#include <iostream>
#include  "a3_avl_tree.hpp"
using namespace std;

bool AVLTree::insert(DataType val){
	Node* nouveau = new Node(val);
	if(getRootNode()==NULL){
		setRootNode(nouveau);
		(increasesize());
		updateNodeBalancetree(getRootNodeAddress());
		return true;
	}
	Node* current=getRootNode();
	while(getRootNode()!=NULL){	
		if(current->val==val){
			return false;
		 }
		else if(current->val<val){
			if(current->right==NULL){
				current->right= nouveau;
				current->right->val =val;
				(increasesize());
				updateNodeBalancetree(getRootNodeAddress());
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
				(increasesize());
				updateNodeBalancetree(getRootNodeAddress());
				return true;
			}
			else{
				current=current->left;
			}
		 }
	}
}
bool AVLTree::remove(DataType val){
	if(size()==0){
		return false;
	}
	if(!(exists(val))){
		return false;
	}
	if(size()==1){
		Node* nouv = NULL;
		setRootNode(nouv);
		(decreasesize());
		updateNodeBalancetree(getRootNodeAddress());
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
			(decreasesize());
			updateNodeBalancetree(getRootNodeAddress());
			return true;
		}
		else if(parent->left == NULL){
			parent->right = NULL;
			delete toberemoved;
			toberemoved = NULL;
			(decreasesize());
			updateNodeBalancetree(getRootNodeAddress());
			return true;
		}
		else if(parent->left->val == val){
			parent->left = NULL;
			delete toberemoved;
			toberemoved = NULL;
			(decreasesize());
			updateNodeBalancetree(getRootNodeAddress());
			return true;
		}
		else if(parent->right->val == val){
			parent->right = NULL;
			delete toberemoved;
			toberemoved = NULL;
			(decreasesize());
			updateNodeBalancetree(getRootNodeAddress());
			return true;
		}
	}
	else if(toberemoved->left!=NULL&&toberemoved->right==NULL&&(toberemoved->left->right==NULL&&toberemoved->left->left==NULL)){
		toberemoved->val = toberemoved->left->val;
		toberemoved->left =NULL;
		(decreasesize());
		updateNodeBalancetree(getRootNodeAddress());
		return true; 
	}
	else if(toberemoved->left==NULL&&toberemoved->right!=NULL&&(toberemoved->right->right==NULL&&toberemoved->right->left==NULL)){
		toberemoved->val = toberemoved->right->val;
		toberemoved->right =NULL;
		(decreasesize());
		updateNodeBalancetree(getRootNodeAddress());
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
		else if(cess->val>getRootNode()->val){
			cess_parent->left = cess->right;
		}
		else if(cess->val<getRootNode()->val){
			cess_parent->right = cess->left;
		}
		(decreasesize());
		updateNodeBalancetree(getRootNodeAddress());
		return true;
	}
}
void AVLTree::updateNodeBalancetree(Node** root){
Node* follow= (*root);
if(follow==NULL){
}
else{
if(follow->right!=NULL){
updateNodeBalancetree(&(follow->right));
}
if(follow->left!=NULL){
updateNodeBalancetree(&(follow->left));
}
updateNodeBalance(follow);
while(follow->avlBalance>1){
	balanceAVLTree(&follow);
	updateNodeBalance(follow);
}
}
/*updateNodeBalance(follow);
while(follow->avlBalance>1){
	balanceAVLTree(&follow);
	updateNodeBalance(follow);
}*/
/*
if(follow->right!=NULL){
updateNodeBalancetree(&(follow->right));
}
if(follow->left!=NULL){
updateNodeBalancetree(&(follow->left));
}
*/
}

void AVLTree::balanceAVLTree(Node** current){
	Node* rent;
	bool done = false;
	if((*current)->right!=NULL){
		if(((*current)->right->left!=NULL)&&((*current)->right->right!=NULL)){
			if(getNodeDepth((*current)->right->left)>getNodeDepth((*current)->right->right)){
				(*current)=search((*current)->right->val,&rent);
				(rent)->right = (rent)->right->left;
				(*current)->left = (rent)->right->right;
				(rent)->right->right=(*current);
				(*current)=search(rent->val,&rent);
				if(rent->val == (*current)->val){
					(*current)=search((*current)->right->val,&rent);
					setRootNode((*current));
					rent->right=(*current)->left;
					(*current)->left = rent;
					done = true;
				}
				else if(rent->right->val == (*current)->val){
					rent->right = (*current)->right;
					(*current)->right = rent->right->left;
					rent->right->left = (*current);
					done = true;
				}
				else if(rent->left->val == (*current)->val){
					rent->left = (*current)->right;
					(*current)->right = rent->left->left;
					rent->left->left = (*current);
					done = true;
				}
			}
			else{
				(*current)=search((*current)->val,&rent);
				if(rent->val == (*current)->val){
					(*current)=search((*current)->right->val,&rent);
					setRootNode((*current));
					rent->right=(*current)->left;
					(*current)->left = rent;
					done = true;
				}
				else if(rent->right->val == (*current)->val){
					rent->right = (*current)->right;
					(*current)->right = rent->right->left;
					rent->right->left = (*current);
					done = true;
				}
				else if(rent->left->val == (*current)->val){
					rent->left = (*current)->right;
					(*current)->right = rent->left->left;
					rent->left->left = (*current);
					done = true;
				}
			}
		}
		else if((*current)->right->left!=NULL){
			(*current)=search((*current)->right->val,&rent);
			(rent)->right = (rent)->right->left;
			(*current)->left = (rent)->right->right;
			(rent)->right->right=(*current);
			(*current)=search(rent->val,&rent);
			if(rent->val == (*current)->val){
				(*current)=search((*current)->right->val,&rent);
				setRootNode((*current));
				rent->right=(*current)->left;
				(*current)->left = rent;
				done = true;
			}
			else if(rent->right->val == (*current)->val){
				rent->right = (*current)->right;
				(*current)->right = rent->right->left;
				rent->right->left = (*current);
				done = true;
			}
			else if(rent->left->val == (*current)->val){
				rent->left = (*current)->right;
				(*current)->right = rent->left->left;
				rent->left->left = (*current);
				done = true;
			}
		}
		else if((*current)->right->right!=NULL){
			(*current)=search((*current)->val,&rent);
			if(rent->val == (*current)->val){
				(*current)=search((*current)->right->val,&rent);
				setRootNode((*current));
				rent->right=(*current)->left;
				(*current)->left = rent;
				done = true;
			}
			else if(rent->right->val == (*current)->val){
				rent->right = (*current)->right;
				(*current)->right = rent->right->left;
				rent->right->left = (*current);
				done = true;
			}
			else if(rent->left->val == (*current)->val){
				rent->left = (*current)->right;
				(*current)->right = rent->left->left;
				rent->left->left = (*current);
				done = true;
			}
		}
	}
	if((*current)->left!=NULL&&done == false){
		if(((*current)->left->right!=NULL)&&((*current)->left->left!=NULL)){
			if(getNodeDepth((*current)->left->right)>getNodeDepth((*current)->left->left)){	
				(*current)=search((*current)->left->val,&rent);
				(rent)->left = (rent)->left->right;
				(*current)->right = rent->left->left;
				(rent)->left->left=(*current);
				(*current)=search(rent->val,&rent);
				if(rent->val == (*current)->val){
					(*current)=search((*current)->left->val,&rent);
					setRootNode((*current));
					rent->left=(*current)->right;
					(*current)->right = rent;
				}
				else if(rent->right->val == (*current)->val){
					rent->right = (*current)->left;
					(*current)->left = rent->right->right;
					rent->right->right = (*current);
					
				}
				else if(rent->left->val == (*current)->val){
		 			rent->left = (*current)->left;
		 			(*current)->left = rent->left->right;
					rent->left->right = (*current);		
				}
			}
			else{
				(*current)=search((*current)->val,&rent);
				if(rent->val == (*current)->val){
					(*current)=search((*current)->left->val,&rent);
					setRootNode((*current));
					rent->left=(*current)->right;
					(*current)->right = rent;
				}
				else if(rent->right->val == (*current)->val){
					rent->right = (*current)->left;
					(*current)->left = rent->right->right;
					rent->right->right = (*current);
					
				}
				else if(rent->left->val == (*current)->val){
		 			rent->left = (*current)->left;
		 			(*current)->left = rent->left->right;
					rent->left->right = (*current);		
				}
					
			}
		}
		else if((*current)->left->right!=NULL){
		(*current)=search((*current)->left->val,&rent);
			(rent)->left = (rent)->left->right;
			(*current)->right = rent->left->left;
			(rent)->left->left=(*current);
			
		(*current)=search(rent->val,&rent);
			if(rent->val == (*current)->val){
				(*current)=search((*current)->left->val,&rent);
				setRootNode((*current));
				rent->left=(*current)->right;
				(*current)->right = rent;
			}
			else if(rent->right->val == (*current)->val){
				rent->right = (*current)->left;
				(*current)->left = rent->right->right;
				rent->right->right = (*current);
				
			}
			else if(rent->left->val == (*current)->val){
	 			rent->left = (*current)->left;
	 			(*current)->left = rent->left->right;
				rent->left->right = (*current);		
			}
		}	
		else if((*current)->left->left!=NULL){
		(*current)=search((*current)->val,&rent);
			if(rent->val == (*current)->val){
				(*current)=search((*current)->left->val,&rent);
				setRootNode((*current));
				rent->left=(*current)->right;
				(*current)->right = rent;
			}
			else if(rent->right->val == (*current)->val){
				rent->right = (*current)->left;
				(*current)->left = rent->right->right;
				rent->right->right = (*current);
				
			}
			else if(rent->left->val == (*current)->val){
	 			rent->left = (*current)->left;
	 			(*current)->left = rent->left->right;
				rent->left->right = (*current);		
			}
		}
	}
}
	



