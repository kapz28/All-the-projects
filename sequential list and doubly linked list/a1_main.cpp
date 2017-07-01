#include <iostream>
#include <string>
#include "a1_tests.hpp"

using namespace std;

string get_status_str(bool status)
{
    return status ? "TEST PASSED" : "TEST FAILED";
}

int main()
{
    SequentialListTest seq_test;
    DoublyLinkedListTest linked_test;

	string seq_test_descriptions[12] = {
      "Test1: New empty list is valid",
      "Test2: insert_front() and insert_back() on zero-element list",
      "Test3: select() and search() work properly",      
	  "Test4: remove_front() and remove_back() on one-element list",
      "Test5: Inserting too many elements should fail",
      "Test6: insert_front() keeps moving elements forward",
      "Test7: inserting at different positions in the list",
      "Test8: try to remove too many elements, then add a few elements",
      "Test9: lots of inserts and deletes, some of them invalid",
      "Test10: inserts into an unsorted list, then sorts the list",      
      "Test11: insert and remove for sorted list in ascending order",
      "Test12: insert and remove for sorted list in descending order"  
	};
	
	bool seq_test_results[12];
    seq_test_results[0] = seq_test.test1();
    seq_test_results[1] = seq_test.test2();
    seq_test_results[2] = seq_test.test3();
    seq_test_results[3] = seq_test.test4();
    seq_test_results[4] = seq_test.test5();
    seq_test_results[5] = seq_test.test6();
    seq_test_results[6] = seq_test.test7();
    seq_test_results[7] = seq_test.test8();
    seq_test_results[8] = seq_test.test9();
    seq_test_results[9] = seq_test.test10();
    seq_test_results[10] = seq_test.test11();
    seq_test_results[11] = seq_test.test12();
    
	cout << "SEQUENTIAL LIST TESTING RESULTS \n";
	cout << "******************************* \n";
	for (int i = 0; i < 12; ++i) {
    	cout << seq_test_descriptions[i] << endl << get_status_str(seq_test_results[i]) << endl;
	}
	cout << endl;


	string linked_test_descriptions[12] = {
      "Test1: New empty list is valid",
      "Test2: insert_front() and insert_back() on zero-element list",
      "Test3: select() and search() work properly",
	  "Test4: remove_front() and remove_back() on one-element list",
      "Test5: replace() works properly",
      "Test6: insert_front() keeps moving elements forward",
      "Test7: inserting at different positions in the list",
      "Test8: try to remove too many elements, then add a few elements",
      "Test9: lots of inserts and deletes, some of them invalid",
      "Test10: inserts into an unsorted list, then sorts the list",      
      "Test11: insert and remove for sorted list in ascending order",
      "Test12: insert and remove for sorted list in descending order"   
	};
	
	bool linked_test_results[12];
    linked_test_results[0] = linked_test.test1();
    linked_test_results[1] = linked_test.test2();
    linked_test_results[2] = linked_test.test3();
    linked_test_results[3] = linked_test.test4();
    linked_test_results[4] = linked_test.test5();
    linked_test_results[5] = linked_test.test6();
    linked_test_results[6] = linked_test.test7();
    linked_test_results[7] = linked_test.test8();
    linked_test_results[8] = linked_test.test9();
    linked_test_results[9] = linked_test.test10();
    linked_test_results[10] = linked_test.test11();
    linked_test_results[11] = linked_test.test12();
	cout << "DOUBLY LINKED LIST TESTING RESULTS \n";
	cout << "********************************** \n";
	for (int i = 0; i < 12; ++i) {
    	cout << linked_test_descriptions[i] << endl << get_status_str(linked_test_results[i]) << endl;
	}
	cout << endl;
    system("pause");

}
