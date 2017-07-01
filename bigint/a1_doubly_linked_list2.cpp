#include <iostream>
#include "a1_doubly_linked_list2.hpp"
#include <sstream>
#include <algorithm>
using namespace std;
DoublyLinkedList::Node::Node(DataType data)//2 pointers to each node and a data value is kept within this node
{ 
	prev= NULL;
	next= NULL;
	value = data;
}

DoublyLinkedList::DoublyLinkedList()// list starts off with no elements thus why pointers are null and size is zero
{
	head_ = NULL;
	tail_ = NULL;
	size_ = 0;
}

DoublyLinkedList::~DoublyLinkedList()//deletes the prev node and keeps going and then deletes the last pointer 
{
	Node *c = head_;
	while (c != NULL)
	{
		Node *g = c;
		c = c->next;
		c->prev = NULL;
		delete g;
	}
	c = NULL;
	delete head_;
	delete tail_;
	
}

unsigned int DoublyLinkedList::size() const
{
	return size_;	//the number of elements the linked list can hold
}

unsigned int DoublyLinkedList::capacity() const
{
	return CAPACITY;    //the max number of elements the linked list can hold
}

bool DoublyLinkedList::empty() const  //if num of elements in list zero then true
{
	if(size_ == 0){
		return true;
	}
	return false;
}

bool DoublyLinkedList::full() const//if num of elements in list  not zero then true
{
	if(size_== CAPACITY){
		return true;
	}
	return false;
}
DoublyLinkedList::DataType DoublyLinkedList::select(unsigned int index) const //returns the value of the node at the given index 
{
	if (index >= size_-1){ // tests to see if input is valid
		DataType num = tail_->value;
		return num;
	}
	else{
		Node *a1 = head_;
		if (index == 0){  //if index 0 then return first nodes value
			DataType num = a1->value;
			return num;	
		}
		for(int i=0;i<index;i++){ //otherwise follows nodes and keep track of index 
			a1 = a1->next;
		}
		DataType num = a1->value;
		return num;
	}
}

unsigned int DoublyLinkedList::search(DataType value) const // returns the index of where the node contains same value as input
{
    Node *a1;
	a1=head_;
	if(head_->value == value){ //check if first node the value
		return 0;
	}
	for(int i=1 ;i<size_;i++){//else do sequential search on list
		a1 = a1->next;
		if(a1->value == value){
			return i;
		}
	}
	return size_;	
}

void DoublyLinkedList::print() const // goes through each node and prints the value each node contains
{
	Node *a = head_;
	cout<<"[";
    while (a != NULL)  //iterate through each node and print while iterating
    {
        cout << a->value << " "; 
        a = a->next; 
    }
    cout<< "]";
    cout << endl;
}

DoublyLinkedList::Node* DoublyLinkedList::getNode(unsigned int index) const // returns the node object which is at the given index
{
		Node *n = head_ ;     
		while(index > 0){   //go to next node and keep track of index
			n = n->next;
			index--;
		}
		return n;  // note return type is the object node
}

//note in the insert functions size++ is done because new node is inserted thus size of elements in list is increas by one


bool DoublyLinkedList::insert(DataType value, unsigned int index) // inserts a node at a given index 
{															      //but does not remove the node that was there
	if (index > size_|| index<0){ //checks for valid input
		return false;
	}else if(index == 0){ // if index 0 then insert in front
		insert_front(value);
		return true;
	}
	
	if (index == size_){ // if index size then insert in back
		insert_back(value);
		return true;	
	}
	Node *newnode = new Node(value);	//note that the prev node and the node after are pointed to the new node
	Node *a1 = getNode(index);			//new node also points their repective prev and next pointers 
	Node *a2 = getNode(index - 1);		//to prev and next node
	newnode -> next = a1;
	a1 ->prev= newnode;
	a2 -> next = newnode;
	newnode -> prev = a2;
	size_ ++;
	return true;
	
}

bool DoublyLinkedList::insert_front(DataType value)//inserts a node in the front of the list
{
	if(size_==CAPACITY){  //if number of elements in list eauls capacity then list full
		return false;
	}
	Node *a1= new Node(value); 
	if(head_==NULL){   //when no elements in list head equals to null
		head_=a1;
		tail_=a1;
		size_++;
		return true;
	}
	else{              
	a1->next =head_;       //new node pointers points to next node
	head_->prev= a1;       //previous first node prev pointer pointed to new node
	head_= a1;			   //head redirected to new node
	size_++;
	return true;
	}
}

bool DoublyLinkedList::insert_back(DataType value)//inserts a node in the back of the list
{												  //follows same logic as insert front
	if(size_==CAPACITY){   
		return false;
	}
	Node *a1= new Node(value);
	if(head_==NULL){
		head_=a1;
		tail_=a1;
		size_++;
		return true;
	}
	else{
	a1->prev = tail_;
	tail_->next= a1;
	tail_= a1;
	size_++;
	return true;	
	}	
}

//note in the remove functions size-- is done 
//because  node is deleted thus size of elements in list is decreased by one

bool DoublyLinkedList::remove(unsigned int index)//removes a node at the given index
{
	if(size_<=0){  //checks for valid input
		return false;
	}
	if(index>=size_){
		return false;
	}
	Node* track = getNode(index);  // returns the node we want to delete
	if(track->prev == NULL){   // this means the last node in list
		return remove_front();
	}
	if(track->next == NULL){  // this means first node in list
		return remove_back();
	}
	track->next->prev = track->prev;  // redirect beside nodes to ignore the node we want to remove 
	track->prev->next = track->next;
	size_--;
	delete track; // delete the node
	track = NULL;
	return true;
}

bool DoublyLinkedList::remove_front()//removes the first node int the list
{ 
	if(size_ ==0 ){ // checks if there are any elements in  the list
		return false;
	}
	if(size_ ==1 ){ //if one then just set head and tail to point to null it will ignore that node
		head_ = NULL;
		tail_ = NULL;
		size_--;
		return true;
	}
	head_ = head_->next; // redirect head to point to second node in list
	size_--;
	return true;
}

bool DoublyLinkedList::remove_back()//removes the last node int the list
{									//same logic as insert_front()
	if(size_ ==0 ){
		return false;
	}
	if(size_ ==1 ){
		head_ = NULL;
		tail_ = NULL;
		size_--;
		return true;
		
	}
	tail_ = tail_->prev; 
	tail_ -> next = NULL; // we do this because other wise it will still point to that node
	size_--;
	return true;
}

bool DoublyLinkedList::replace(unsigned int index, DataType value)//accesses the node at the given index and changes the value with input
{
	if (index > size_-1){ // checks valid input
		return false;
	}
	else if(index==0){  //in the case of one node in list
	 head_->value= value;
	 return true;
	}
	else{
	Node *a1;
	a1=head_;
	for(int i=0;i<index;i++){		//track node in list
		a1 = a1->next;
	}   
	a1->value = value; // change value
	return true;
	}
}

bool DoublyLinkedList::is_sorted_asc() const { //checks to see if the values in the list are in ascending order
	if(size_ == 0 || size_ ==1){ // if one node or zero then sorted ascended
		return true;
	}else {
		Node *a1 = head_;
		Node* a2 = a1->next;
		for(int i=0 ;i<(size_-1);i++){  // checks to see if prev value less than next value
			if(a1->value > a2->value){
				return false;// if not then list is not ascending
			}
			a1 = a1->next;
			a2 = a1->next;
		}
	}
	return true;	
}

bool DoublyLinkedList::is_sorted_desc() const { //checks to see if the values in the list are in descending order
	if(size_ == 0 || size_ ==1){  // same logic as is_sorted_asc()
		return true;
	}
	Node *a1 = head_;
	Node* a2 = a1->next;
	if (size_>2){
		for(int i=0 ;i<(size_-1);i++){
			if(a1->value < a2->value){
				return false;
			}
			a1 = a1->next;
			a2 = a1->next;
		}
	}
	return true;	
}

bool DoublyLinkedList::insert_sorted_asc(DataType val) { //inserts a node with value in a ascending list that keeps it ascending
	if (!is_sorted_asc()){ //checks to see if list is in ascending order
		return false;
	}else if(size_ == 0){ //if no elements in list 
		insert_front(val);
		return true;
	}
	else{
		for(int i = 0; i < size_; i++){
			if (getNode(i)->value >= val){ // checks to see where it fits in the list to keep it ascending
				insert(val,i);
				return true;
			}
		}
	insert_back(val);
	return true;	
	}
}

bool DoublyLinkedList::insert_sorted_desc(DataType val) { //inserts a node with value in a descending list that keeps it ascending
	if (!is_sorted_desc()){								// same logic as insert_sorted_asc()
		return false;
	}else if(size_ == 0){
		insert_front(val);
		return true;
	}
	else{
		for(int i = 0; i < size_; i++){
			if (getNode(i)->value <= val){
				
				insert(val,i);
				return true;
			}
			}
	
	insert_back(val);
	return true;	
	}
}

void DoublyLinkedList::sort_asc() {   //sorts the values in the list in ascending order
	if(size_==0||size_==1){	//these cases there is nothing to be done
	}
	else{
	Node *a1;
	a1=head_;
	Node* a2 = a1->next;
		for(int j=0 ;j<(size_-1);j++){  //bubble sort sorts each value in array 
			a1= head_;				    //two nodes need to compare values
			a2 = a1->next;
			for(int i=0 ;i<(size_-1);i++){
				if(a1->value>a2->value){ // checks to see  if in ascending order when not then switches values
						DataType temp = a2->value;
						a2->value=a1->value;
						a1->value=temp;
				}
				a1 = a1->next;
				a2 = a1->next;
			}
	    }
	}
}
    
void DoublyLinkedList::sort_desc() { //sorts the values in the list in ascending order
	if(size_==0||size_==1){			//same logic as sort_asc()
	}
	else{
	Node *a1;
	a1=head_;
	Node* a2 = a1->next;
		for(int j=0 ;j<(size_-1);j++){
			a1= head_;
			a2 = a1->next;
			for(int i=0 ;i<(size_-1);i++){
				if(a1->value<a2->value){
						DataType temp = a2->value;
						a2->value=a1->value;
						a1->value=temp;
				}
				a1 = a1->next;
				a2 = a1->next;
			}
	    }
	}
}


