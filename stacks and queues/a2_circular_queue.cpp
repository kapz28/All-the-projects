#include <iostream>
#include "a2_circular_queue.hpp"
using namespace std;
typedef CircularQueue::QueueItem QueueItem;
const QueueItem CircularQueue::EMPTY_QUEUE = -999;

CircularQueue::CircularQueue()
{
	capacity_ = 16; 
	size_ = 0; 
	head_ = 0;
	tail_ = 0;
	items_ = new QueueItem[capacity_]; 
	
}

CircularQueue::CircularQueue(unsigned int capacity)
{
	capacity_ = capacity; 
	size_ = 0; 
	head_ = 0;
	tail_ = 1;
	items_ = new QueueItem[capacity_]; 	
}

CircularQueue::~CircularQueue()
{
	delete [] items_; 
}

bool CircularQueue::empty() const
{    
	if(size_==0)
		return true; 
	return false; 
}

bool CircularQueue::full() const
{
	if(size_==capacity_)
		return true; 
	else return false; 
}

int CircularQueue::size() const
{  
	return size_; 
}

bool CircularQueue::enqueue(QueueItem value)
{
	if(full()){
		return false; 
	}
	
	items_[tail_] = value; 
	tail_ = (tail_+1)%capacity_; 
	
	size_++; 
	return true; 
}

QueueItem CircularQueue::dequeue()
{
	if(empty())
		return CircularQueue::EMPTY_QUEUE; 
		
	size_--; 
	QueueItem temp = items_[head_]; 
	head_= (head_+1)%capacity_; 
	return temp; 
}

QueueItem CircularQueue::peek() const
{
	if(size_==0){
		return CircularQueue::EMPTY_QUEUE; 
	}
	return items_[head_]; 
	
}

void CircularQueue::print() const
{
	cout<<"[";
	for(int i=head_;i<size_;i++){
	  cout<<items_[i]<<",";	
	}
	cout<<"]"<<endl;
	cout<<"capacity is"<<capacity_<<endl;
}


