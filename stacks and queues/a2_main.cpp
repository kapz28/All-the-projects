
#include <iostream>
#include <string>
using namespace std;

#include "a2_tests.hpp"

string get_status_str(bool status)
{
    return status ? "TEST PASSED" : "TEST FAILED";
}

int main() {
    DynamicStackTest stack_test;
    CircularQueueTest queue_test;

	string stack_test_descriptions[10] = {
      "Test1: New stack is valid",
      "Test2: Push one item and then pop it",
      "Test3: Simple push, pop, and peek test",
      "Test4: Push 4 items, peek 5 items and then pop 5 items",
      "Test5: Full stack doubles properly",
      "Test6: Full stack doubles, then full stack doubles",
      "Test7: Push a lot, pop a lot, no halving",
      "Test8: Push a lot, pop a lot, halving",
      "Test9: Lots of pushes, peeks, and pops",
      "Test10: Lots of pushes, peeks, and pops"
	};
	
	bool stack_test_results[10];
    stack_test_results[0] = stack_test.test1();
    stack_test_results[1] = stack_test.test2();
    stack_test_results[2] = stack_test.test3();
    stack_test_results[3] = stack_test.test4();
    stack_test_results[4] = stack_test.test5();
    stack_test_results[5] = stack_test.test6();
    stack_test_results[6] = stack_test.test7();
    stack_test_results[7] = stack_test.test8();
    stack_test_results[8] = stack_test.test9();
    stack_test_results[9] = stack_test.test10();

	cout << "DYNAMIC STACK TESTING RESULTS \n";
	cout << "***************************** \n";
	for (int i = 0; i < 10; ++i) {
    	cout << stack_test_descriptions[i] << endl << get_status_str(stack_test_results[i]) << endl;
	}
	cout << endl;
	
	string queue_descriptions[10] = {
      "Test1: New queue is valid",
      "Test2: Enqueue one item and then dequeue it",
      "Test3: Enqueue two items and then dequeue three",
      "Test4: Enqueue 16 items, peek 3 items and then dequeue 18 items",
      "Test5: Enqueue items onto a full queue returns false",
      "Test6: Queue is circular for enqueue()",
      "Test7: Fill queue, then empty queue",
      "Test8: Queue is circular for dequeue()",
      "Test9: Lots of enqueues, dequeues, peeks, all valid",
      "Test10: Lots of enqueues, dequeues, peeks, not all valid"
	};
    
	bool queue_test_results[10];
    queue_test_results[0] = queue_test.test1();
    queue_test_results[1] = queue_test.test2();
    queue_test_results[2] = queue_test.test3();
    queue_test_results[3] = queue_test.test4();
    queue_test_results[4] = queue_test.test5();
    queue_test_results[5] = queue_test.test6();
    queue_test_results[6] = queue_test.test7();
    queue_test_results[7] = queue_test.test8();
    queue_test_results[8] = queue_test.test9();
    queue_test_results[9] = queue_test.test10();

	cout << "CIRCULAR QUEUE TESTING RESULTS \n";
	cout << "****************************** \n";
	for (int i = 0; i < 10; ++i) {
    	cout << queue_descriptions[i] << endl << get_status_str(queue_test_results[i]) << endl;
	}
	cout << endl;
    system("pause");
}
