//Kapilan Satkunananthan and Sanjeevani Lakshmivarahan
#include <iostream>
#include "a2_dynamic_stack.hpp"
using namespace std;
typedef DynamicStack::StackItem StackItem;  // for simplicity
const StackItem DynamicStack::EMPTY_STACK = -999;

DynamicStack::DynamicStack() // initializes a dynamic array of size 16 
{
	capacity_= 16;
	size_=0;
	init_capacity_=16;
	items_= new  StackItem[init_capacity_];
}

DynamicStack::DynamicStack(unsigned int size_) //initializes a dynamic array of size inputed
{
	items_= new StackItem[size_];
	capacity_= size_;
	this->size_=0;
	init_capacity_=size_;
	
}

DynamicStack::~DynamicStack() // deletes the pointer items* 
{
	delete []items_;
}

bool DynamicStack::empty() const //if no items in array then true else false
{
	if(size_==0){
	   return true;
	}
	return false;
		
}

int DynamicStack::size() const //return number of items in the array
{
	return size_;
}

void DynamicStack::push(StackItem value)//push an item at the end of array
{
	if(size_<capacity_){  // if room available then push normally
		items_[size_] = value;
		size_++;
	}
	else{	// else create a new array with twice the capacity copy previous array then push
	StackItem* temp= new  StackItem[capacity_*2];
		for(int i=0;i<capacity_;i++){
			temp[i]=items_[i];
		}
		capacity_*=2;
		items_=temp;
		items_[size_] = value;
		size_++;
	}
}

StackItem DynamicStack::pop()
{
	if(empty()){ // if empty return emptystack constant
		return DynamicStack::EMPTY_STACK;
	}
	StackItem toreturn= items_[size_-1];      //store the value to be popped
	size_--;  //"delete" last item in stack
	
	if(size_<=capacity_/4.0 && capacity_/2.0>=init_capacity_){  //if number of items is less than quarter of capacity 
								    //and also if half the capacity is bigger than the initial capacity
		StackItem* temp= new  StackItem[capacity_/2];  //then halve the capacity of the stack
		for(int i=0;i<size_;i++){
			temp[i]=items_[i];			//copying old stack into new halved stack
		}
		items_=temp;
		capacity_ = capacity_/2; 
	}
	return toreturn;
}

StackItem DynamicStack::peek() const
{
	if(size_==0){    //empty case scenario
		return DynamicStack::EMPTY_STACK;      
	}
	return items_[size_-1];  // else return most recent "pushed item" in stack
}

void DynamicStack::print() const
{
	cout<<"[";
	for(int i=0;i<size_;i++){    //iterate through each item in stack and print it 
	  cout<<items_[i]<<",";	
	}
	cout<<"]"<<endl;
	cout<<"capacity is"<<capacity_<<endl;
}

