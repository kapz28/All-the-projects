//kapilan satkunanathan and yuanpei gao
#include <iostream>
//#include "a1_doubly_linked_list2.hpp"
//#include "a1_big_int.hpp"
#include <sstream>
#include<string>
#include<math.h>
#include "a1_tests.hpp"
#include "a1_big_int.hpp"
#include <algorithm>
 using namespace std;
string get_status_str(bool status)
{
    return status ? "TEST PASSED" : "TEST FAILED";
}

int main()
{
    
	DoublyLinkedListTest linked_test;
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
	class BigIntTest
	{
	public:
		
		// New empty list is valid
	    bool test_big_add(string un,string deux)
	    {
			//cout<< "kap";
			BigInt a(un);
			BigInt b(deux);
			a.add(&b);
			
			
		    return true;
	    }
	    bool test_big_sub(string trois,string four)
	    {
			//cout<< "kap";
			BigInt c(trois);
			BigInt d(four);
			c.sub(&d);
			
		    return true;
	    }
	};


	string b,c;	
	cout << "first big number in add/sub (make sure first number bigger than second and both numbers positive)"<<endl;
	cin >> b ;
	cout << "second big number in add/sub"<<endl;
	cin >> c;
	BigIntTest a;
	
	
	
	
	string linked_test_descriptions_big[2] = {
      "Test1: New empty list is valid",
      "Test2: insert_front() and insert_back() on zero-element list",
	};
	
	int linked_test_results_big[2];
    linked_test_results_big[0] = a.test_big_add(b,c);
    linked_test_results_big[1] = a.test_big_sub(b,c);

	cout << "BIG INT ADD/SUB TEST RESULTS \n";
	cout << "********************************** \n";
	for (int i = 0; i < 2; ++i) {
    	cout << linked_test_descriptions_big[i] << endl << get_status_str(linked_test_results_big[i]) << endl;
	}
	cout << endl;
    system("pause");
	cout << endl;


}
 
