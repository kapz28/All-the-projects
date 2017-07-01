#include<iostream>
#include "a1_big_int.hpp"
#include<stdio.h>     
#include<math.h>
#include<string> 
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

BigInt::BigInt(bool B[], int size)
{
	
	double store=size;
	DataType sum=0;
	int bruh=0;
	a=new DoublyLinkedList();
	if(size==0 || size<0){
	}	
	for(int i=0;i<(size/32+1);i++){
		sum = 0;
		for(int j=1;j<33;j++){
			if((size-j-i*32)>=0){
			 sum += B[size-(j+(i*32))] * pow(2,(j-1));
			 cout <<sum << endl;
			  if(j==32){
			   a->insert_front(sum);
			   }
			}
			else{
			 if(bruh==0&&sum!=0){
					a->insert_front(sum);
					bruh++;
			 }
			}
		}
	}
		a->print();
	}


BigInt::BigInt(string value){
	a=new DoublyLinkedList();
	int size = value.size();
	int i = 0;
	string bin = "";
	int index = 0; 
	while(i < value.size()){//i = 0, size = 3, 
		int last = 0;
		int r = 0;
		char l;
		//cout << "a"<< endl;
		
		for (int j = index; j < size; j++){
			int num = value.at(j) - '0' + last*10;//num = 1 = 12
			if (num == 1 && j == size-1){
				l = '1';
				value[size-1] = '0';
				continue;
			}else if(num == 1 ){
				value[j]= '0';
				j ++;// j = 2
				num = 10 + (value.at(j) - '0');
			}	
			last = num %2;//last = 0
			//cout <<"num" << num<< endl;
			int NewN = num / 2;//num = 6
			//cout <<"NewN" << NewN<< endl;
			//cout << "substr" << value[j] << endl;
			value[j] = NewN + '0'; // 6
			//cout << "substr" << value[j] << endl;			
			l = last + '0';
		
		}
		//cout << "last" << l<< endl;
		bin = l + bin;
		//cout << 
		
		//cout <<"bin" << bin << endl;
		
		if (value[i] - '0' == 0){
			//cout << "c" << endl;
			index ++;
			i ++;
			
			//cout << "i" << i << endl;
			//cout << size << endl;
		}
		int test = 0;
		//cout << value << endl;
		//cout << i;
		//cout << value.size()<< endl;
		
	}
	
	int size2 = bin.size();
	while (size2 > 32){
		string s = bin.substr(size2-33,32);
		unsigned int b = bin2dec(s);
		a->insert_front(b);
		size2 -= 32;
	}
	
	string final = bin.substr(0,size2);
	unsigned int f = bin2dec(final);
	a->insert_front(f);
	
	a->print();
	
}

string BigInt::dec2bin(int n) 
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

unsigned int BigInt::bin2dec(string str){
	if (!str.size())
		return 0;
	return bin2dec(str.substr(1))+(str.at(0) - '0')*pow(2,str.size()-1);
}

unsigned int BigInt::ascii2int(string str){
	if(!str.size()){
		return 0;
	}else{
		return ascii2int(str.substr(1))+(str.at(0)-'0')*pow(10,str.size()-1);
	}	
} 


BigInt* BigInt::add(BigInt* rhs) 
{
}
BigInt* BigInt::sub(BigInt* rhs) 
{

}
BigInt* BigInt::mul(BigInt* rhs) 
{
/*	BigInt* answer;
	BigInt* d= a;
	DataType h;
	for(int i=0;i<h;i++){
		add(d);
	}
	answer=a;
	return answer;
	*/
}

BigInt* BigInt::and_(BigInt* rhs) 
{
	DataType h,b,c;
	b=a->select(10);
	for(int i=0;i<a->size();i++){
		h=rhs->a->select(i);
		b=a->select(i);
		c=(h & b);
		a->remove(i);
		a->insert(i,c);
	}
	a->print();
}
BigInt* BigInt::or_(BigInt* rhs)
{
		DataType h,b,c;
	for(int i=0;i<a->size();i++){
		h=rhs->a->select(i);
		b=a->select(i);
		c=(h | b);
		a->remove(i);
		a->insert(i,c);
	}
}
BigInt* BigInt::xor_(BigInt* rhs) 
{
	DataType h,b,c;
	for(int i=0;i<a->size();i++){
		h=rhs->a->select(i);
		b=a->select(i);
		c=(h ^ b);
		a->remove(i);
		a->insert(i,c);
	}
}
