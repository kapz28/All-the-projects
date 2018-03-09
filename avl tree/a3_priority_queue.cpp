#include <iostream>
#include  "a3_priority_queue.hpp"

using namespace std;

PriorityQueue::PriorityQueue(unsigned int capacity){
	heap_ = new DataType[capacity+1];
	capacity_ = capacity;
	size_ = 0;

}
PriorityQueue::~PriorityQueue() {
	delete [] heap_;
}
unsigned int PriorityQueue::size() const {
	return size_;
}
bool PriorityQueue::empty() const {
	if(!size_){
		return true;
	}
	return false;
}
bool PriorityQueue::full() const{
	if(size_==capacity_){
	  return true;
	}
	return false;
}
void PriorityQueue::print() const{
	cout<<"[ ,";
	for(int i=1;i<capacity_+1;i++){
		cout<<heap_[i]<<",";
	}
	cout<<"] is the queue"<<endl;
}
PriorityQueue::DataType PriorityQueue::max() const{
	return heap_[1];
}
bool PriorityQueue::enqueue(DataType val) {

	if(size_==capacity_){
		return false;
	}	
	for(int i=size_;i>0;i--){
		heap_[i+1]=heap_[i];
	}
	heap_[1]=val;
	size_++;
	int i, j,temp;
	for (i = 1; i < size_; i++)
	{
		for (j = i+1; j < size_+1; j++)
		{
			if (heap_[i] < heap_[j])
			{
				temp = heap_[j];
				heap_[j] = heap_[i];
				heap_[i] = temp;
			}	
		}
	}	

	return true;
}
bool PriorityQueue::dequeue() {
	if(!size_){
		return false;
	}
	for(int i=1;i<capacity_+1;i++){
		heap_[i]=heap_[i+1];
	}
	size_--;
	return true;

}

