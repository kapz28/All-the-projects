//kapilan satkunanathan and yuanpei gao
#include <iostream>
#include "a1_doubly_linked_list.hpp"

using namespace std;
DoublyLinkedList::Node::Node(DataType data)
{ 
	prev= NULL;
	next= NULL;
	value = data;
}

DoublyLinkedList::DoublyLinkedList()
{
	head_ = NULL;
	tail_ = NULL;
	size_=0;
}

DoublyLinkedList::~DoublyLinkedList()
{
}

unsigned int DoublyLinkedList::size() const
{
	return size_;
}

unsigned int DoublyLinkedList::capacity() const
{
	return CAPACITY;
}

bool DoublyLinkedList::empty() const
{
//	if(head_==NULL){
	if(size_==0){
		return true;
	}
	return false;
}

bool DoublyLinkedList::full() const
{
	if(size_== CAPACITY){
		return true;
	}
	return false;
}
DoublyLinkedList::DataType DoublyLinkedList::select(unsigned int index) const
{
	if (index >= size_-1){
		DataType num = tail_->value;
		return num;
	}else{
	
	Node *a1 = new Node(0);
	a1=head_;
	for(int i=0;i<index;i++){
		a1 = a1->next;
	}
	DataType num = a1->value;
	return num;
}
}

unsigned int DoublyLinkedList::search(DataType value) const
{
    Node *a1;
	a1=head_;
	if(head_->value == value){
		return 0;
	}
	for(int i=1 ;i<size_;i++){
		a1 = a1->next;
		if(a1->value == value){
			return i;
		}
	}
	return size_;	
}

void DoublyLinkedList::print() const
{
	Node *a = head_; //current is the beginning value of the list
    while (a != NULL) //Continue while current is not at the end of the list
    {
        cout << a->value << " "; //Print out the data of current
        a = a->next; //Move one value along in the list
    }
}

DoublyLinkedList::Node* DoublyLinkedList::getNode(unsigned int index) const
{
		Node *n = head_;
		while(index > 0){
			n = n->next;
			index--;
		}
		return n;
}

bool DoublyLinkedList::insert(DataType value, unsigned int index)
{
	if (index > size_|| index<0){
		return false;
	}
	if(index == 0 && size_==0){
		Node *newnode = new Node(value);
		head_ =newnode;
		tail_ = newnode;
		size_ ++;
		return true;
	}
	if (index == size_){
		insert_back(value);
		return true;	
	}
	if(size_==1 && index == 0){
		Node *newnode = new Node(value);
		head_ =newnode;
		newnode->next= tail_;
		tail_->prev = newnode;
		size_ ++;
		return true;	
	}

	Node *newnode = new Node(value);
	Node *a1 = getNode(index);
	Node *a2 = getNode(index - 1);
	newnode -> next = a1;
	a1 ->prev= newnode;
	a2 -> next = newnode;
	newnode -> prev = a2;
	size_ ++;
	return true;
}

bool DoublyLinkedList::insert_front(DataType value)
{
	Node *a1= new Node(value);
	if(head_==NULL){
		head_=a1;
		tail_=a1;
		size_++;
		return true;
	}
	else{
	a1->next =head_;
	head_->prev= a1;
	head_= a1;
	size_++;
	return true;
	}
}

bool DoublyLinkedList::insert_back(DataType value)
{
	
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

bool DoublyLinkedList::remove(unsigned int index)
{
	if(size_==0){
		return false;
	}
	if(index>=size_){
		return false;
	}
	if(index<0){
		return false;
	}
	Node* track = getNode(index);
	if(track->prev == NULL){
		return remove_front();
	}
	if(track->next == NULL){

		return remove_back();
	}
	track->next->prev = track->prev;
	track->prev->next = track->next;
	size_--;
	delete track;
	track = NULL;
	return true;
}

bool DoublyLinkedList::remove_front()
{ 
	if(size_ ==0){
		return false;
	}
	if(size_ ==1){
		head_ = NULL;
		tail_ = NULL;
		size_--;
		return true;
	}
	head_ = head_->next;
	size_--;
	return true;
}

bool DoublyLinkedList::remove_back()
{
	if(size_ ==0 ){
		return false;
	}
	if(size_ ==1 ){
		head_ = NULL;
		tail_ = NULL;
		size_--;
		return true;
		
	}
	Node *kap= tail_;
	tail_ = tail_->prev;
	tail_->next=NULL;
	size_--;
	return true;
}

bool DoublyLinkedList::replace(unsigned int index, DataType value)
{
	if (index > size_-1){
		return false;
	}
	else if(index==0){
	 head_->value= value;
	 return true;
	}
	else{
	Node *a1;
	a1=head_;
	//Node* temp;
	for(int i=0;i<index;i++){
		//temp = a1;
		a1 = a1->next;
	}
	a1->value = value;
	return true;
	}
}

bool DoublyLinkedList::is_sorted_asc() const {
	if(size_ == 0 || size_ ==1){
		return true;
	}
	Node *a1;
	a1=head_;
	Node* a2 = a1->next;
	for(int i=0 ;i<(size_-1);i++){
		if(a1->value > a2->value){
			return false;
		}
		a1 = a1->next;
		a2 = a1->next;
	}
	return true;	
}

bool DoublyLinkedList::is_sorted_desc() const {
	if(size_ == 0 || size_ ==1){
		return true;
	}
	Node *a1;
	a1=head_;
	Node* a2 = a1->next;
	for(int i=0 ;i<(size_-1);i++){
		if(a1->value < a2->value){
			return false;
		}
		a1 = a1->next;
		a2 = a1->next;
	}
	return true;		
}

bool DoublyLinkedList::insert_sorted_asc(DataType val) {
	if(!is_sorted_asc()){
		return false;
	}
	if(size_==0){
		insert_front(val);
		return true;	
	}
	if(tail_->value <= val){
		insert_back(val);
		return true;
	}
	if(head_->value > val){
		insert_front(val);
		return true;
	}
	if(size_==1){
			DataType temp= head_->value;
			head_->value = val;
			insert_back(temp);  
			return true;
	}
	Node *a1;
	a1=head_;
	Node* a2 = a1->next;
	for(int i=0 ;i<(size_-1);i++){
		if((a1->value<val)&&(a2->value > val)){
			if(a1->value == a2->value){
			} 
			else{
				DataType index=search(a2->value);
				insert(val,index);
				return true;
			}
		}
		a1 = a1->next;
		a2 = a1->next;
	}
}

bool DoublyLinkedList::insert_sorted_desc(DataType val) {
	if(!is_sorted_desc()){
		return false;
	}
	if(size_==0){
		insert_front(val);
		return true;	
	}
	if(tail_->value >= val){
		insert_back(val);
		return true;
	}
	if(head_->value < val){
		insert_front(val);
		return true;
	}
	Node *a1;
	a1=head_;
	Node* a2 = a1->next;
	for(int i=0 ;i<(size_-1);i++){
		if((a1->value>val)&&(a2->value < val)){
			if(a1->value == a2->value){
			} 
			else{
				DataType index=search(a2->value);
				insert(val,index);
				return true;
			}
		}
		a1 = a1->next;
		a2 = a1->next;
	}
}

void DoublyLinkedList::sort_asc() {
	if(size_==0||size_==1){	
	}
	else{
	Node *a1;
	a1=head_;
	Node* a2 = a1->next;
	for(int j=0 ;j<(size_-1);j++){
		a1= head_;
		a2 = a1->next;
		for(int i=0 ;i<(size_-1);i++){
			if(a1->value>a2->value){
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
    
void DoublyLinkedList::sort_desc() {
	if(size_==0||size_==1){	
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


