//kapilan satkunanathan and yuanpei gao
#include "a1_sequential_list.hpp"
#include <iostream>
using namespace std;
SequentialList::SequentialList(unsigned int cap) //new dynamic array created 
{
  data_ = new DataType[cap];
  capacity_= cap; //max elements it can hold
  size_=0; // # of elements it has
}

SequentialList::~SequentialList() //deletes the container
{
	delete []data_;
}

unsigned int SequentialList::size() const
{
	return size_;  // number of elements in container
}

unsigned int SequentialList::capacity() const
{
	return capacity_;	// max elements container can hold
}

bool SequentialList::empty() const // if  size not zero false else true
{
	
	if (size_!= 0){
		return false;
    }
	else{
	 return true;
	}
}

bool SequentialList::full() const //if  size  zero false else true
{
		if (size_== capacity_){
		return true;
    }
	else{
	 return false;
	}
}

SequentialList::DataType SequentialList::select(unsigned int index) const
{
	if (index >= capacity_)
		return data_[size_-1];  //when not valid input then returns last element in container
	else
		return data_[index]; // returns the element at index
}

unsigned int SequentialList::search(DataType val) const
{
		for(int i = 0; i < size_; i++) { //sequential search through container for value
            if(data_[i] == val){
            	return i;
			}
        }
        return capacity_;
}

void SequentialList::print() const // prints out each element in array as iterating thru array
{
	cout<<"["; 
	    for(int i = 0; i < capacity_; i++) {
            cout<<data_[i]<<" ";
        }
    cout<<"]"<<endl;    
}

bool SequentialList::insert(DataType val, unsigned int index)
{
	if(index>size_){ //checks for valid input
		return false;
	}
	if(size_==capacity_){ //checks for valid input
		return false;
	}
	else{
	for (int i = size_; i > index; i --){
		data_[i] = data_[i-1]; // shifts elements to the right leaving space for the one to be inserted
	}
	size_+=1;
	data_[index]=val;
	return true;
    }
}

bool SequentialList::insert_front(DataType val)
{
	if(size_==capacity_){ // checks for valid input
		return false;
	}
	for (int i = size_; i > 0; i --){
		data_[i] = data_[i-1]; // shifts elements to the right leaving space for the one to be inserted
	}
	data_[0] = val;
	
	size_= size_ + 1;
	return true;
}

bool SequentialList::insert_back(DataType val)
{
	if(size_==capacity_){ // checks for valid input
		return false;
	}
	data_[size_] = val; // in this case nothing needs to be shifted so just direct insert
	
	size_= size_ + 1;
	return true;
}

bool SequentialList::remove(unsigned int index)
{ 
	if(size_==0||index>=size_){ // checks for valid input
		return false;
	}
	data_[index] = 0;
	for (int i = index; i < capacity_; i ++){
		data_[i] = data_[i+1]; //shift elements to the left value at index will get replaced
	}
	size_= size_ - 1;
	return true;
}

bool SequentialList::remove_front()
{
	if(size_==0){ // checks for valid input
		return false;
	}
	for (int i = 0; i < capacity_; i ++){
		data_[i] = data_[i+1]; //shift elements to the left first one will get replaced
	}
	size_= size_ - 1;
	return true;
}

bool SequentialList::remove_back()
{
	if(size_==0){ //checks for valid inputs
		return false;
	}
	data_[size_-1]= 0; // no need to shift just remove back element
	size_= size_ - 1;
	return true;	
}

bool SequentialList::replace(unsigned int index, DataType val)
{
	data_[index] =val; // very easy as no removing just replacing just need to directly access and replace
}

bool SequentialList::is_sorted_asc() const {
	for(int i = 0; i<(size_-1); i++){
		if(data_[i]>data_[i+1]){ // checks to see if element before is smaller than next element in array
		return false;
		}	
	}
return true;
}

bool SequentialList::is_sorted_desc() const { // same logic as is_sorted_asc()
	for(int i = 0; i<(size_-1); i++){
		if(data_[i]<data_[i+1]){ // checks to see if element before is larger than next element in array
		return false;
		}	
	}
	return true;
}

bool SequentialList::insert_sorted_asc(DataType val) {
	if (size_==0){ // in the case where no elements just insert at index 0
		data_[0]=val;
		size_++;
		return true;
	}
	if(!is_sorted_asc()){ // checks to see if list sorted 
		return false;
	}
	else{
		for(int i = 0; i < size_; i++){
			if (data_[i] > val){ // sees where the value would be inserted when  value shifted to right is larger than input
				insert(val, i);
				return true;
			}
		}
		insert_back(val); // if value bigger than all elements just insert at back
		return true;
	}
}

bool SequentialList::insert_sorted_desc(DataType val) { // same logic as insert_sorted_asc()
	if (size_==0){ // in the case where no elements just insert at index 0
		data_[0]=val;
		size_++;
		return true;
	}
	if (!is_sorted_desc()){ // checks to see if list sorted 
		return false;
	}
	else{
		for(int i = 0; i < size_; i++){
			if (data_[i] < val){ // sees where the value would be inserted when  value shifted to right is smaller than input
				insert(val, i);
				return true;
			}
		}
		insert_back(val);
		return true;
	}
}
void SequentialList::sort_asc() {
    int temp;
    for (int j = 0; j < size_ - 1; j++) {  // bubble sort is being done 
		for (int i = 0; i < size_ - 1; i++) { 
	        if (data_[i] > data_[i + 1]) { // checks if value before is greater than value after
	            temp = data_[i];            // if true swap
	            data_[i] = data_[i + 1];   // this done for every element in container
	            data_[i + 1] = temp;
	        }
		}
		
	}
}
void SequentialList::sort_desc() {
	int temp;
    for (int j = 0; j < size_ -1; j++) {  // bubble sort is being done
		for (int i = size_ -1 ; i >= 0 ; i--) {
	        if (data_[i] < data_[i + 1]) {  // checks if value before is greater than value after
	            temp = data_[i];			// if true swap
	            data_[i] = data_[i + 1];	// this done for every element in container
	            data_[i + 1] = temp;
	        }
		}
	}
}


