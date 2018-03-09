#include "a3_priority_queue.hpp"

PriorityQueue::PriorityQueue(unsigned int capacity) {
	heap_ = new DataType[capacity + 1];
	capacity_ = capacity;
	size_ = 0;
	
}

unsigned int PriorityQueue::size() const {
	return size_;
}

bool PriorityQueue::full() const {
	return size_ == capacity_;
}

bool PriorityQueue::empty() const {
	return !size_;
}

bool PriorityQueue::enqueue(DataType val) {
	// if full, return false
	if(full())
		return false;
	
	// set value at right-most leaf of heap equal to val
	heap_[++size_] = val;
	
	int i = size_;
	// travel towards root, checking if val is greater than parent
	while(i != 1 && heap_[i] > heap_[i / 2]) {
		// swap val with parent
		int temp = heap_[i];
		heap_[i] = heap_[i / 2];
		heap_[i / 2] = temp;
		// make index equal to parent index
		i /= 2;
	}
	
	return true;
}

PriorityQueue::DataType PriorityQueue::max() const {
	// if queue is empty, return -1
	if(empty())
		return -1;
	
	// otherwise, return root
	return heap_[1];
}

bool PriorityQueue::dequeue() {
	// if empty, return false
	if(empty())
		return false;
		
	// make root equal to right-most leaf
	heap_[1] = heap_[size_];
	
	// decrement size
	size_--;
	
	int i = 1;
	
	// ensure that i is not a leaf and that i is less than at least one of its children
	while((size_ >= (2 * i) + 1) && (heap_[i] < heap_[2 * i] || heap_[i] < heap_[2 * i + 1])) {
		// find greater child of i and swap with i
		if(heap_[2 * i] > heap_[2 * i + 1]) {
			int temp = heap_[i];
			heap_[i] = heap_[2 * i];
			heap_[2 * i] = temp;
			i *= 2;
		}
		else {
			int temp = heap_[i];
			heap_[i] = heap_[2 * i + 1];
			heap_[2 * i + 1] = temp;
			i *= 2;
			i++;
		}
	}
	
	return true;
}

PriorityQueue::~PriorityQueue() {
	delete [] heap_;
}
