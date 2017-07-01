#ifndef A1_TESTS_HPP
#define A1_TESTS_HPP
#include <iostream>
#include "a1_doubly_linked_list2.hpp"
#include "a1_big_int.hpp"

#define ASSERT_TRUE(T) if (!(T)) return false;
#define ASSERT_FALSE(T) if ((T)) return false;

using namespace std;
class BigIntTest
{
public:
	//"Test1: BigInt created",  ,1,1,1,1,1,1,1,1,1,1    11111111111111111111111111111111
    bool test1()
    {
    	bool kap[1]={1};
        BigInt t= BigInt(kap,1);
        ASSERT_TRUE(t.get()->select(0)==1)
        return true;
    }

	//"Test2: BigInt bool array constructor works",
      
    bool test2()
    {
    	bool kap[64]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
        BigInt t= BigInt(kap,64);
        ASSERT_TRUE(t.get()->select(0)==4294967295)
        ASSERT_TRUE(t.get()->select(1)==4294967295)
        return true;
    }

	//"Test3: BigInt string constructor works",      

    bool test3()
    {
    	BigInt t= BigInt("18446744073709551615");
        ASSERT_TRUE(t.get()->select(0)==4294967295)
        ASSERT_TRUE(t.get()->select(1)==4294967295)
        return true;
    }
    
    //"Test4: add function works",

    bool test4(){
	   int a=64;
	   bool input[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   bool input2[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.add(&f);
	    ASSERT_TRUE(t.get()->select(0)==0)
        ASSERT_TRUE(t.get()->select(1)==0)
        ASSERT_TRUE(t.get()->select(2)==2)
       return true;
	   
    }
    
    //"Test5: vigorous testing of add works", 2305843009213694000

    bool test5()
    {
	   int a=60;
	   bool input[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   bool input2[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.add(&f);
		ASSERT_TRUE(t.get()->select(0)==48)
        ASSERT_TRUE(t.get()->select(1)==536870912)
       return true;	
	
	}

    //       "Test6: sub function works",
 
    bool test6()
    {
	   int a=60;
	   bool input[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   bool input2[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.sub(&f);
	    ASSERT_TRUE(t.get()->select(0)==0)
       return true;
	}
	
	//      "Test7: vigorous testing of sub works",

    bool test7()
    {
  	   int a=60;
	   bool input[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   bool input2[a]={1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.sub(&f);
	    ASSERT_TRUE(t.get()->select(0)==0)
	    ASSERT_TRUE(t.get()->select(0)==178257920)
       return true;
	}
	
	//       "Test8: multiply function works",

    bool test8()
    {
       int a=2;
	   bool input[a]={1,0};
	   bool input2[a]={1,1};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.mul(&f);
	   ASSERT_TRUE(t->get()->select(0)==6)
       return true;
	}
	
	//       "Test9: vigorous testing of multiply works",

	bool test9() {
  	   int a=30;
	   bool input[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   bool input2[a]={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.mul(&f);
	   ASSERT_TRUE(t->get()->select(0)==4227858432)
	   ASSERT_TRUE(t->get()->select(1)==1073741823)
	    return true;
	}
	
		//       "Test10: and works",      
      
	bool test10() {
	   int a=60;
	   bool input[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   bool input2[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.and_(&f);
	    ASSERT_TRUE(t.get()->select(0)==4294967295)
	    ASSERT_TRUE(t.get()->select(1)==268435455)
       return true;
	}
	
		// "Test11: or works",
      
	bool test11() {
	   int a=60;
	   bool input[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   bool input2[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.or_(&f);
	    ASSERT_TRUE(t.get()->select(0)==4294967295)
	    ASSERT_TRUE(t.get()->select(1)==268435455)
       return true;
	}
	
	  //"Test12: xor works"
	bool test12() {
		   int a=60;
	   bool input[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   bool input2[a]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	   BigInt d = BigInt(input,a);
	   BigInt f = BigInt(input2,a);
	   BigInt* t = d.xor_(&f);
	    ASSERT_TRUE(t.get()->select(0)==0)
	    ASSERT_TRUE(t.get()->select(1)==0)
       return true;
	}

};


#endif
