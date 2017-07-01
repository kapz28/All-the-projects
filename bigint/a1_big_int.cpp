#include<iostream>
#include "a1_big_int.hpp"
#include<stdio.h>     
#include<math.h>
#include<string> 
#include <sstream>
#include <algorithm>

using namespace std;

BigInt::BigInt(bool B[], int size)
{

	
	double store=size;
	DataType sum=0;
	int bruh=0;
	a=new DoublyLinkedList();
	if(size==0 || size<0){
		//cout << "Hello";
	}	
	for(int i=0;i<(size/10+1);i++){
		sum = 0;
		for(int j=1;j<12;j++){
				if((size-(j+(i*10)))>=0){
				cout<<"index is "<<(size-(j+(i*10)))<<" b is "<<B[size-(j+(i*10))]<<" sum is "<<sum<<" j is "<<j<<" pow is "<<pow(10,(j-1))<<endl;
				if(j==11){
					a->insert_back(sum);
				}
				sum += B[size-(j+(i*10))] * pow(10,(j-1));
				}
				else{
					if(bruh==0&&sum!=0){
						a->insert_back(sum);
						bruh++;
					}
				}
			}
		//cout<<"banana sum is"<<sum;
	
		}
		a->print();
	DataType temp;
	int storeb=store;
	int storea;
	storeb=(storeb/10)+1;
	if(storeb%2==0){
	storea=((storeb/2));
	}
	else{
	storea=((storeb/2)+1);	
	}
    store=ceil(store/10);
    int o=0;
    do{
        temp=a->select(store);
        a->remove(store);
        a->insert(a->select(o),store);
        a->remove(o);
        a->insert(temp,o);
        o++;
		storea--;
        store--;    
    }while(storea>0);
    a->remove_front();
	a->print();
	}
	/*
	DataType sum=0;
	int bruh=1;
	a=new DoublyLinkedList();
	if(size==0 || size<0){
		cout << "Hello";
	}	
	for(int i=0;i<(size/10+1);i++){
		sum = 0;
		for(int j=1;j<12;j++){
				if((size-(j+(i*10)))>=0){
				cout<<"index is "<<(size-(j+(i*10)))<<" b is "<<B[size-(j+(i*10))]<<" sum is "<<sum<<" j is "<<j<<" pow is "<<pow(10,(j-1))<<endl;
				sum += B[size-(j+(i*10))] * pow(10,(j-1));
				}
			}
		cout<<"banana sum is"<<sum;
		a->insert_front(sum);	
		}
	a->print();
	*/		
	//}
	

BigInt::BigInt(string value){
	/*DataType a = ascii2int(value);
	string b = dec2bin(a);
	int s = b.size();
	DataType bnum = ascii2int(b);
	DataType times = s / 32;
	if(s/32==0){
		times=1;
	}
	a = new DoublyLinkedList();
	for(int i=0;i<(times+1);i++){
		DataType n = bnum % pow(10,32);
		bnum = bnum / pow(10,32);
		a->insert_front(n);
	}
	a->print();
	*/
}
/*
string BigInt::dec2bin(int n) const
{
	if(n==0){
		return "0";		
	}
	else if (n == 1){
		return "1";
	}else{
		return dec2bin(n/2)+(n%2?"1":"0");
	}
}

DataType BigInt::ascii2int(string str){
	if(!str.size()){
		return 0;
	}else{
		return ascii2int(str.substr(1))+(str.at(0)-'0')*pow(10,str.size()-1);
	}	
} 
bool BigInt::ascii2bool(string str){
	if(!str.size()){
		return 0;
	}else{
		return ascii2int(str.substr(1))+(str.at(0)-'0')*pow(10,str.size()-1);
	}	
} */
/*
BigInt* BigInt::add(BigInt* rhs) const
{
}
BigInt* BigInt::sub(BigInt* rhs) const
{
	return size_;
}
BigInt* BigInt::mul(BigInt* rhs) const
{
	return size_;
}
BigInt* BigInt::and_(BigInt* rhs) const
{
	return size_;
}
BigInt* BigInt::or_(BigInt* rhs) const
{
	return size_;
}
BigInt* BigInt::xor_(BigInt* rhs) const
{
	return size_;
}
*/
