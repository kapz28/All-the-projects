#ifndef A2_TESTS_HPP
#define A2_TESTS_HPP

#define ASSERT_TRUE(T) if (!(T)) return false;
#define ASSERT_FALSE(T) if ((T)) return false;

#include "a2_dynamic_stack.hpp"
#include "a2_circular_queue.hpp"

class DynamicStackTest
{
public:
	// New stack is valid
    bool test1() {
	      unsigned int size = 24;
	
	      DynamicStack default_size_stack;
	      ASSERT_TRUE(default_size_stack.empty() == true)
	      ASSERT_TRUE(default_size_stack.size() == 0)    
	
	      DynamicStack stack(size);
	      ASSERT_TRUE(stack.empty() == true)
	      ASSERT_TRUE(stack.size() == 0)    
	      return true;
    }

	// Push one item and then pop it
    bool test2() {
	      DynamicStack stack;
	      stack.push(10);
	      ASSERT_TRUE(stack.empty() == false)
	      ASSERT_TRUE(stack.size() == 1)       
	      ASSERT_TRUE(stack.pop() == 10);
	      ASSERT_TRUE(stack.empty() == true)
	      ASSERT_TRUE(stack.size() == 0)     
	      return true;
    }

	// Simple push, pop, and peek test
    bool test3() {
	      DynamicStack stack(24);
	      stack.push(10);
	      stack.push(20);
	      ASSERT_TRUE(stack.peek() == 20)     
	      ASSERT_TRUE(stack.pop() == 20)
	      ASSERT_TRUE(stack.pop() == 10)     
	      ASSERT_TRUE(stack.pop() == DynamicStack::EMPTY_STACK)
	      ASSERT_TRUE(stack.peek() == DynamicStack::EMPTY_STACK)
	      return true;
    }
    
    // Push 4 items, peek 5 items and then pop 5 items
	bool test4() {
	     DynamicStack stack(8);
	     stack.push(10);
	     stack.push(20);
	     stack.push(30);
	     stack.push(40);
	     //stack.print();
	     ASSERT_TRUE(stack.peek() == 40)
	     ASSERT_TRUE(stack.peek() == 40)
	     ASSERT_TRUE(stack.peek() == 40)
	     ASSERT_TRUE(stack.peek() == 40)
	     ASSERT_TRUE(stack.peek() == 40)                    
	     ASSERT_TRUE(stack.pop() == 40)
	     ASSERT_TRUE(stack.pop() == 30)
	     ASSERT_TRUE(stack.pop() == 20)
	     ASSERT_TRUE(stack.pop() == 10)
	     ASSERT_TRUE(stack.pop() == DynamicStack::EMPTY_STACK)
	     return true;
	}
	
	// Full stack doubles properly
	bool test5() {
	     DynamicStack stack;
	     for(int i = 0; i < 17; i++)
	             stack.push(i);
	     ASSERT_TRUE(stack.capacity_ == 32)
	     for(int i = 16; i >= 0; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     ASSERT_TRUE(stack.pop() == DynamicStack::EMPTY_STACK)
	     return true;
	}
	
	// Full stack doubles, then full stack doubles
	bool test6() {
	     DynamicStack stack(8);
	     for(int i = 0; i < 17; i++)
	             stack.push(i);
	     ASSERT_TRUE(stack.capacity_ == 32)
	     for(int i = 16; i >= 0; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     ASSERT_TRUE(stack.pop() == DynamicStack::EMPTY_STACK)
	     return true;
	}
	
	// Push a number of items equal to initial size and then remove most of them so that no. of items 
	//is less than half the size of the stack. As the size limit is still the initial size, no 
	//changes in its value should take place
	bool test7() {
	     DynamicStack stack;
	     for(int i = 0; i < 16; i++)
	             stack.push(i);
	     ASSERT_TRUE(stack.capacity_ == 16)
	     for(int i = 15; i >= 1; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     ASSERT_TRUE(stack.capacity_ == 16)
	     ASSERT_TRUE(stack.size() == 1)
	     return true;
	}
	
	// Push a number of items equal to initial size + 1 and then remove slightly more than half of them so that 
	//no. of items is less than half the new size of the stack. The size limit should be halved by then
	bool test8() {
	     DynamicStack stack;
	     for(int i = 0; i < 17; i++)
	             stack.push(i);
	     ASSERT_TRUE(stack.capacity_ == 32)
	     for(int i = 16; i >= 6; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     ASSERT_TRUE(stack.capacity_ == 16)
	     return true;
	}
	
	//Check this sequence of pushes, peeks and pops
	bool test9() {
	     DynamicStack stack(8);
	     for(int i = 0; i < 10; i++)
	             stack.push(i);
	     ASSERT_TRUE(stack.capacity_ == 16)
	     ASSERT_TRUE(stack.size() == 10)
	     ASSERT_TRUE(stack.peek() == 9)
	     for(int i = 9; i >= 6; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     for(int i = 6; i < 36; i++)
	             stack.push(i);
	     ASSERT_TRUE(stack.capacity_ == 64)
	     ASSERT_TRUE(stack.size() == 36)
	     ASSERT_TRUE(stack.peek() == 35)
	     ASSERT_TRUE(stack.peek() == 35)
	     for(int i = 35; i >= 0; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     ASSERT_TRUE(stack.pop() == DynamicStack::EMPTY_STACK)
	     ASSERT_TRUE(stack.pop() == DynamicStack::EMPTY_STACK)
	     return true;
	}
	
	// Check this sequence of pushes, peeks and pops
	bool test10() {
	     DynamicStack stack(4);
	     stack.push(0);
	     stack.push(1);
	     ASSERT_TRUE(stack.size() == 2)
	     for(int i = 2; i < 5; i++)
	             stack.push(i);
	     for(int i = 4; i >= 3; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     ASSERT_TRUE(stack.size() == 3)
	     ASSERT_TRUE(stack.capacity_ == 8)
	     ASSERT_TRUE(stack.pop() == 2)
	     ASSERT_TRUE(stack.pop() == 1)
	     ASSERT_TRUE(stack.capacity_ == 4)
	     ASSERT_TRUE(stack.peek() != 1)
	     ASSERT_TRUE(stack.peek() == 0)     
	     ASSERT_TRUE(stack.pop() == 0)     
	     ASSERT_TRUE(stack.size() == 0)
	     for(int i = 0; i < 65; i++)
	             stack.push(i);
	     ASSERT_TRUE(stack.size() == 65)
	     ASSERT_TRUE(stack.capacity_ == 128)
	     for(int i = 64; i >= 30; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     ASSERT_TRUE(stack.capacity_ == 64)
	     for(int i = 29; i >= 1; i--)
	             ASSERT_TRUE(stack.pop() == i)
	     ASSERT_TRUE(stack.capacity_ == 4)
	     return true;
	}

};

class CircularQueueTest
{
public:
	// New queue is valid
    bool test1() {
	      CircularQueue queue;
	      ASSERT_TRUE(queue.empty() == true)
	      ASSERT_TRUE(queue.full() == false)
	      ASSERT_TRUE(queue.size() == 0)
	      ASSERT_TRUE(queue.size_ == 0)
	      ASSERT_TRUE(queue.capacity_ == 16)     
	      return true;
    }

	// Enqueue one item and then dequeue it
    bool test2() {
	      CircularQueue queue;
	      ASSERT_TRUE(queue.enqueue(10) == true)
	      ASSERT_TRUE(queue.empty() == false)
	      ASSERT_TRUE(queue.full() == false)
	      ASSERT_TRUE(queue.size_ == 1)
	      ASSERT_TRUE(queue.head_ == 0)
	      ASSERT_TRUE((queue.tail_ == 1) || (queue.tail_ == 2))
	      ASSERT_TRUE(queue.dequeue() == 10);
	      ASSERT_TRUE(queue.empty() == true)
	      ASSERT_TRUE(queue.size_ == 0)
	      return true;
    }

	// Enqueue two items and then dequeue three
    bool test3() {
	      CircularQueue queue;
	      ASSERT_TRUE(queue.enqueue(10) == true)
	      ASSERT_TRUE(queue.enqueue(20) == true)     
	      ASSERT_TRUE(queue.empty() == false)
	      ASSERT_TRUE(queue.full() == false)
	      ASSERT_TRUE(queue.size_ == 2)
	      ASSERT_TRUE(queue.head_ == 0)
	      ASSERT_TRUE((queue.tail_ == 2) || ((queue.tail_ == 3)))
	      ASSERT_TRUE(queue.peek() == 10);  
	      ASSERT_TRUE(queue.dequeue() == 10);
	      ASSERT_TRUE(queue.peek() == 20);  
	      ASSERT_TRUE(queue.dequeue() == 20);
	      ASSERT_TRUE(queue.peek() == CircularQueue::EMPTY_QUEUE);  
	      ASSERT_TRUE(queue.dequeue() == CircularQueue::EMPTY_QUEUE);     
	      ASSERT_TRUE(queue.empty() == true)
	      ASSERT_TRUE(queue.size_ == 0)
	      ASSERT_TRUE(queue.head_ == 2)
	      ASSERT_TRUE((queue.tail_ == 2) || (queue.tail_ == 3))
	      return true;
    }
    
    // Enqueue 16 items, peek 3 items and then dequeue 18 items
	bool test4() {
	     CircularQueue queue;
	     for(int i = 0; i < 16; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     ASSERT_TRUE(queue.peek() == 0)
	     ASSERT_TRUE(queue.peek() == 0)
	     ASSERT_TRUE(queue.peek() == 0)          
	     ASSERT_TRUE(queue.size_ == 16)
	     ASSERT_TRUE(queue.head_ == 0)
	     ASSERT_TRUE((queue.tail_ == 15) || ((queue.tail_ == 0)))
	     ASSERT_TRUE(queue.full() == true)
	     for(int i = 0; i < 16; i++)
	             ASSERT_TRUE(queue.dequeue() == i)
	     ASSERT_TRUE(queue.empty() == true)
	     ASSERT_TRUE(queue.dequeue() == CircularQueue::EMPTY_QUEUE)
	     ASSERT_TRUE(queue.dequeue() == CircularQueue::EMPTY_QUEUE)
	     return true;
	}
	
	// Make sure the queue is circular when using the enqueue() function
	bool test5() {
	     CircularQueue queue;     
	     for(int i = 0; i < 16; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     ASSERT_TRUE(queue.dequeue() == 0)
	     ASSERT_TRUE(queue.dequeue() == 1)
	     ASSERT_TRUE(queue.dequeue() == 2)
	     ASSERT_TRUE(queue.enqueue(16) == true)
	     ASSERT_TRUE(queue.enqueue(17) == true)
	     ASSERT_TRUE(queue.enqueue(18) == true)
	     ASSERT_TRUE(queue.full() == true)
	     ASSERT_TRUE(queue.head_ == 3)
	     ASSERT_TRUE((queue.tail_ == 2) || ((queue.tail_ == 3)))  
	     return true;
	}
	
	// Enqueue items onto a full queue returns false
	bool test6() {
	     CircularQueue queue;
	     for(int i = 0; i < 16; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     ASSERT_TRUE(queue.enqueue(16) == false)
	     ASSERT_TRUE(queue.head_ == 0)
	     ASSERT_TRUE((queue.tail_ == 15) || ((queue.tail_ == 0)))     
	     return true;
	}
	
	// Fill queue, then empty queue
	bool test7() {
	     CircularQueue queue;     
	     for(int i = 0; i < 16; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     ASSERT_TRUE(queue.full() == true)                  
	     for(int i = 0; i < 16; i++)
	             ASSERT_TRUE(queue.dequeue() == i)
	     ASSERT_TRUE(queue.dequeue() == CircularQueue::EMPTY_QUEUE)             
	     ASSERT_TRUE(queue.empty() == true)
	     ASSERT_TRUE(queue.head_ == 0)
	     ASSERT_TRUE((queue.tail_ == 15) || ((queue.tail_ == 0)))
	     return true;
	}
	
	// Make sure the queue is circular when using the dequeue() function
	bool test8() {
	     CircularQueue queue;
	     for(int i = 0; i < 16; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     for(int i = 0; i < 10; i++)
	             ASSERT_TRUE(queue.dequeue() == i)
	     for(int i = 0; i < 6; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     for(int i = 0; i < 6; i++)
	             ASSERT_TRUE(queue.dequeue() == i + 10)
	     ASSERT_TRUE(queue.dequeue() == 0)
	     ASSERT_TRUE(queue.dequeue() == 1)
	     ASSERT_TRUE(queue.dequeue() == 2)
	     return true;
	}
	
	// Check this sequence of enqueues, dequeues and peeks; all of them valid
	bool test9() {
	     CircularQueue queue;
	     for(int i = 0; i < 12; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     for(int i = 0; i < 10; i++)
	             ASSERT_TRUE(queue.dequeue() == i)
	     for(int i = 0; i < 4; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     ASSERT_TRUE(queue.dequeue() == 10)
	     ASSERT_TRUE(queue.dequeue() == 11)
	     ASSERT_TRUE(queue.enqueue(5))
	     ASSERT_TRUE(queue.enqueue(6))
	     ASSERT_TRUE(queue.dequeue() == 0)
	     for(int i = 0; i < 11; i++)
	             ASSERT_TRUE(queue.enqueue(i))
	     ASSERT_TRUE(queue.full() == true)
	     return true;
	}
	
	// Check this sequence of enqueues, dequeues and peeks; some of them invalid
	bool test10() {
	     CircularQueue queue;
	     for(int i = 0; i < 7; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     for(int i = 0; i < 5; i++)
	             ASSERT_TRUE(queue.dequeue() == i)
	     ASSERT_TRUE(queue.head_ == 5)
	     ASSERT_TRUE((queue.tail_ == 6) || ((queue.tail_ == 7)))     
	     for(int i = 0; i < 14; i++)
	             ASSERT_TRUE(queue.enqueue(i) == true)
	     ASSERT_TRUE(queue.full() == true)
	     ASSERT_TRUE(queue.enqueue(20) == false)
	     ASSERT_TRUE(queue.dequeue() == 5)
	     ASSERT_TRUE(queue.dequeue() == 6)
	     ASSERT_TRUE(queue.enqueue(14))
	     ASSERT_TRUE(queue.enqueue(15))
	     ASSERT_TRUE(queue.dequeue() == 0)
	     ASSERT_TRUE(queue.head_ == 8)
	     ASSERT_TRUE((queue.tail_ == 6) || ((queue.tail_ == 7)))
	     for(int i = 1; i < 16; i++)
	             ASSERT_TRUE(queue.dequeue() == i)
	     ASSERT_TRUE(queue.empty() == true)
	     ASSERT_TRUE(queue.head_ == 7)
	     ASSERT_TRUE(queue.dequeue() == CircularQueue:: EMPTY_QUEUE)
	     return true;
	}
};


#endif
