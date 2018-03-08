//kapilan satkunanathan and yuanpei gao
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
	for(int i=0;i<(size/32+1);i++){
		sum = 0;
		for(int j=1;j<33;j++){
			if((size-j-i*32)>=0){
			sum += B[size-(j+(i*32))] * pow(2,(j-1));
			//cout <<sum << endl;
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
		//cout<<"banana sum is"<<sum;
	}
		a->print();
	
}


BigInt::BigInt(string value){
	a=new DoublyLinkedList();
	int size = value.size();
	int i = 0;
	string bin = "";
	int index = 0; 
	while(i < value.size()){
		int last = 0;
		int r = 0;
		char l;
		for (int j = index; j < size; j++){
			int num = value.at(j) - '0' + last*10;
			if (num == 1 && j == size-1){
				l = '1';
				value[size-1] = '0';
				continue;
			}else if(num == 1 ){
				value[j]= '0';
				j ++;
				num = 10 + (value.at(j) - '0');
			}	
			last = num %2;
			
			int NewN = num / 2;
			value[j] = NewN + '0'; 
			l = last + '0';
		}
		bin = l + bin;
		if (value[i] - '0' == 0){
			index ++;
			i ++;	
		}
		int test = 0;
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
	
	
}

string BigInt::dec2bin(unsigned int n) 
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

string BigInt::int2ascii(unsigned int num){
	int d = num / 10;
	int r = num % 10;
	char last = r + '0';
	string l = "";
	l = l + last;
	if (d == 0){
		return l;
	}else{
		return int2ascii(d) + l;
	}
} 
BigInt* BigInt::print(){
	this->a->print();	
}
BigInt* BigInt::add(BigInt* rhs) {
	int size1 = this->a->size();
	int size2 = rhs->a->size();
	int max = 0;
	int min = 0;
	// get max size
	if (size1>=size2){
		max = size1;
		min = size2;	
	}else{
		max = size2;
		min = size1;
	}
	int prevNum = 0;
	string strNew = "";
	//do addition from the last digit and storing the result as a binary string
	for (unsigned int i = max; i >0 ; i--){
		unsigned int num1 = 0;
		if (a->select(i)){
			num1 = a->select(i);
		}
		unsigned int num2 = 0;
		if(rhs->a->select(i)){
			num2 = rhs->a->select(i);
		}
		string str1 = dec2bin(num1);
		str1 = string(32 - str1.size(),'0' ) + str1;
		string str2 = dec2bin(num2);
		str2 = string(32 - str2.size(),'0' ) + str2;
			for (int j = 31; j >= 0; j--){
			int last = str1.at(j) - '0' + str2.at(j) - '0' + prevNum;
			prevNum = 0;
			while (last >= 2){
				prevNum = 1;
				last -=2; 
			}
			char l = last + '0';
			strNew = l + strNew ;
		//	cout << "strNew" << strNew << endl;
			}
	
	}
	//check the first digit
	if (prevNum == 1){
		strNew = '1' + strNew;
	}
	//convert to boolean array and use constructor to store it
	int sss=strNew.size();
	bool final[sss];
	for ( int i = 0; i < sss; i++){
		final[i] = strNew.at(i)-'0';
	}
	
	BigInt* sum =new BigInt(final,sss);
	return sum;
}

BigInt* BigInt::sub(BigInt* rhs) 
{
	int size1 = this->a->size();
	int size2 = rhs->a->size();
	int max = 0;
	int min = 0;
	// get the max node and see which bigint is larger
	int situation = 0;
	if (size1>=size2){
		max = size1;
		min = size2;
		situation = 1;
		if(max == min){
			if ((a->select(0)) < (rhs->a->select(0))){
				situation = 2;
			}else{
				situation = 1;
			}
		}	
	}else{
		max = size2;
		min = size1;
		situation = 2;
	}
	int prevNum = 0;
	string strNew = "";
	//use the larger one minus the smaller one, depent on the value of situation
	if (situation == 1){
	for (unsigned int i = max; i >0 ; i--){
		unsigned int num1 = 0;
		if (a->select(i)){
			num1 = a->select(i);
		}
		unsigned int num2 = 0;
		if(rhs->a->select(i)){
			num2 = rhs->a->select(i);
		}
		string str1 = dec2bin(num1);
		str1 = string(32 - str1.size(),'0' ) + str1;
		string str2 = dec2bin(num2);
		str2 = string(32 - str2.size(),'0' ) + str2;
			for (int j = 31; j >= 0; j--){
			int last = str1.at(j) - '0' - str2.at(j) + '0' - prevNum;
			prevNum = 0;
			while (last < 0){
				prevNum = 1;
				last +=2; 
			}
			char l = last + '0';
			strNew = l + strNew ;
			}
	}
}else if(situation == 2){
	for (unsigned int i = max; i >0 ; i--){
		unsigned int num1 = 0;
		if (a->select(i)){
			num1 = a->select(i);
		}
		unsigned int num2 = 0;
		if(rhs->a->select(i)){
			num2 = rhs->a->select(i);
		}
		string str1 = dec2bin(num1);
		str1 = string(32 - str1.size(),'0' ) + str1;
		string str2 = dec2bin(num2);
		str2 = string(32 - str2.size(),'0' ) + str2;
			for (int j = 31; j >= 0; j--){
			int last = str2.at(j) - '0' - str1.at(j) + '0' - prevNum;
			prevNum = 0;
			while (last < 0){
				prevNum = 1;
				last +=2; 
			}
			char l = last + '0';
			strNew = l + strNew ;
			}
	}
}
	//check the first digit
	if (prevNum == 1){
		strNew = strNew.substr(1);
	}
	//convert to boolean array and use constructor to store it
	int sss=strNew.size();
	bool final[sss];
	for ( int i = 0; i < sss; i++){
		final[i] = strNew.at(i)-'0';
	}	
	BigInt* dif =new BigInt(final,sss);
	return dif;
}

BigInt* BigInt::mul(BigInt* rhs) 
{
	unsigned int size = rhs->a->size();
	unsigned int sizeA = this->a->size();
	string base = "";
	string multi = "";
	//convert this to binary string
	for (int i = 0; i < sizeA; i++ ){
		base = base + dec2bin(this->a->select(i)) ;
	}
	//convert rhs to binary string
	for (int i = 0; i < size; i++ ){
		multi = multi + dec2bin(rhs->a->select(i));
	}
	int sizeOne = base.size();
	int sizeT = multi.size();
	BigInt* result = new BigInt("0");
	for (unsigned int i = 0; i <sizeT ; i++){
		//completing base
		bool factor[sizeOne + i ];
		for(int j = 0; j < sizeOne + i; j++){
		    factor[j] = 0;
		}
		for ( int j = 0; j < sizeOne; j++){
			factor[j] = base.at(j)-'0';
		}
		BigInt* one = new BigInt(factor,sizeOne+i);
		if (multi[sizeT-1-i] - '0' == 1){
			result = result->add(one);
		}
				
			
	}
	
	return result;
}

DoublyLinkedList* BigInt::get(){
	return this->a;
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
	BigInt* d =this;
	return d;
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
	BigInt* d =this;
	return d;
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
	BigInt* d =this;
	return d;
}


