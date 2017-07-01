#ifndef A1_BIG_INTHPP
#define A1_BIG_INTHPP
#include<iostream>
#include<string>
#include "a1_doubly_linked_list2.hpp"
#include <stdio.h>    
#include <math.h> 
#include <sstream>
#include <algorithm>
using namespace std;
class BigInt {
public:
    // Can be seen outside as DoublyLinkedList::DataType
    typedef unsigned int DataType;

private:
friend class BigIntTest;

DoublyLinkedList *a;
public:
	// CONSTRUCTOR/DESTRUCTOR
    // Create a new empty DoublyLinkedList.
    ~BigInt();
    DoublyLinkedList* get(); 
    BigInt(bool B[], int size); // takes a boolean and array and size of array and makes a bigint
    BigInt(string value); 
    BigInt* add(BigInt* rhs);
    BigInt* sub(BigInt* rhs);
    BigInt* mul(BigInt* rhs);
    BigInt* and_(BigInt* rhs);
    BigInt* or_(BigInt* rhs);
    BigInt* xor_(BigInt* rhs);
    DataType bin2dec(string str);
    string dec2bin(unsigned int n);
    DataType ascii2int(string str);
    string int2ascii(unsigned int num);
};
#endif 
