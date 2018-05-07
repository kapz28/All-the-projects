//kapilan satkunanathan and yuanpei gao
#include <iostream>
#include "middleend.hpp"
#include <string>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>   
    

using namespace std;
Game::Game(){
	
}
Game::Game(int boardsize,char mode, int*** pos_point,char*** ele_point) 
{
	board_size = boardsize;
	int boardsizec = boardsize;
	if(mode =='P'){
		int** ary = new int*[boardsizec];
  		for(int i = 0; i < boardsizec; ++i){
      		ary[i] = new int[boardsizec];
      		}
		char** ary_two = new char*[boardsizec];
  		for(int i = 0; i < boardsizec; ++i){
      		ary[i] = new char[boardsizec];
      		}
		int index = 0;
		pos_point = &ary;
		ele_point = &ary_two;
		for(int i = 0; i<boardsize;i++){
			for(int j=0; j<boardsize;j++){
				pos_point**[i][j] = index;
				index++;	
			}
		}
		for(int i = 0; i<boardsize;i++){
			for(int j=0; j<boardsize;j++){
				int generate_element= rand() % 10; 
				//generate elements for board
				if(generate_element>=0 && generate_element <2){
					ele_point**[i][j] = 'O'; 	
				}
				else if(generate_element>=2 && generate_element <4){
					ele_point**[i][j] = 'P';
				}
				else if(generate_element>=4 && generate_element <6){
					ele_point**[i][j] = 'N';
				}
				else if(generate_element>=6 && generate_element < 8){
					ele_point**[i][j] = '+';
				}
				else if(generate_element>=8 && generate_element < 10){
					ele_point**[i][j] = '-';
				}	
			}
		}	
	ele_point**[0][0] = 'A';
	ele_point**[board_size-1][board_size-1] = 'B';
	// add player part here 
	Player one;
	Player two;
	one.position = 0;
	two.position = ((board_size*board_size)-1);
	}
	
}

Game::~Game()
{
}   
unsigned int Game::size() const
{
	return board_size;
}

bool Game::end_game_cond() const
{
	if(one.get_health() == 0 || two.get_health() == 0|| get_num_items() == 0|| two.position == one.position){
		if(one.get_health() ==0){
			cout<<"player 2 wins!";
		}
		else if(two.get_health() ==0){
			cout<<"player 1 wins!";
		}
		else if(one.points >=two.points){
			cout<<"player 1 wins!";
		}
		else if(two.points >=one.points){
			cout<<"player 2 wins!";
		}
		else{
			cout<<"it's a draw";
		}
		return true;
	} 
	return false;
}

bool Game::update_board(char move_player_one,char move_player_two) const
{ 
		if(move_player_one == 'W'){
			if(!((one.position) +10) > (board_size*board_size-1)){
				ele_point**[one.position] = 'O'; //[one.position/board_size][one.position%board_size]
				one.position = one.position +10; 
				if(ele_point**[one.position/board_size][one.position%board_size] == '+'){
					one.points += 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == '-'){
					one.points -= 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == 'N'){
					one.health -= 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == 'P'){
					one.health += 10;
				}
				return true;	
			}
			return false;
		}
		else if(move_player_one == 'A'){
			if(!((one.position%board_size==0))){
				one.position = one.position -10; 
				ele_point**[one.position/board_size][one.position%board_size] = 'O';
				if(ele_point**[one.position/board_size][one.position%board_size] == '+'){
					one.points += 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == '-'){
					one.points -= 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == 'N'){
					one.health -= 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == 'P'){
					one.health += 10;
				}
				return true;	
			}
			return false;

		}
		else if(move_player_one == 'S'){
			if(!((one.position) - 10) < (board_size*board_size)){
				one.position = one.position - 10; 
				ele_point**[one.position/board_size][one.position%board_size] = 'O';
				if(ele_point**[one.position/board_size][one.position%board_size] == '+'){
					one.points += 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == '-'){
					one.points -= 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == 'N'){
					one.health -= 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == 'P'){
					one.health += 10;
				}
				return true;	
			}
			return false;
		}
		else if(move_player_one == 'D'){
			if(!((one.position%board_size==(board_size-1)))){
				one.position = one.position+1; 
				ele_point**[one.position/board_size][one.position%board_size] = 'O';
				if(ele_point**[one.position/board_size][one.position%board_size] == '+'){
					one.points += 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == '-'){
					one.points -= 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == 'N'){
					one.health -= 10;
				}
				else if(ele_point**[one.position/board_size][one.position%board_size] == 'P'){
					one.health += 10;
				}
				return true;	
			}
			return false;
		}
		if(move_player_two == 'I'){
			if(!((two.position) +10) > (board_size*board_size)){
				two.position = two.position +10; 
				ele_point**[two.position/board_size][two.position%board_size] = 'O';
				if(ele_point**[two.position/board_size][two.position%board_size] == '+'){
					two.points += 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == '-'){
					two.points -= 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == 'N'){
					two.health -= 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == 'P'){
					two.health += 10;
				}
				return true;	
			}
			return false;
		}
		else if(move_player_two == 'J'){
			if(!((two.position%board_size==0))){
				two.position = two.position -10; 
				ele_point**[two.position/board_size][two.position%board_size] = 'O';
				if(ele_point**[two.position/board_size][two.position%board_size] == '+'){
					two.points += 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == '-'){
					two.points -= 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == 'N'){
					two.health -= 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == 'P'){
					two.health += 10;
				}
				return true;	
			}
			return false;
		}
		else if(move_player_two == 'K'){
			if(!((two.position) -10) < (board_size*board_size)){
				two.position = two.position -10; 
				ele_point**[two.position/board_size][two.position%board_size] = 'O';
				if(ele_point**[two.position/board_size][two.position%board_size] == '+'){
					two.points += 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == '-'){
					two.points -= 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == 'N'){
					two.health -= 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == 'P'){
					two.health += 10;
				}
				return true;	
			}
			return false;
		}
		else if(move_player_two == 'L'){
			if(!((two.position%board_size==(board_size-1)))){
				two.position = two.position+1; 
				ele_point**[two.position/board_size][two.position%board_size] = 'O';
				if(ele_point**[two.position/board_size][two.position%board_size] == '+'){
					two.points += 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == '-'){
					two.points -= 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == 'N'){
					two.health -= 10;
				}
				else if(ele_point**[two.position/board_size][two.position%board_size] == 'P'){
					two.health += 10;
				}
				return true;	
			}
			return false;
		}
}
int Game::get_num_items() const
{
	int count = 0; 
		for(int i = 0; i<board_size;i++){
			for(int j=0; j<board_size;j++){
				if(!(ele_point**[i][j]=='O'||ele_point**[i][j]=='A'||ele_point**[i][j]=='B')){
					count++;	
				}		
			}
		}
	return count;
}
void Game::print(){
    //for statement to print values using array             
    for( int i=0; i < board_size; ++i ){
    	cout<<"[";
	    for( int j=0; j < board_size; ++j ){
			cout<< ele_point**[i][j];
		}
		cout<<"]"<<endl;
		
	}      	
}
Player::Player()
{
	health = 100;
	points = 0;
	
}
Player::~Player()
{
	
}

// get health 
int Player::get_health(){
	return health;
}
	
// modify health 
int Player::health_modify(int val){
	health = health + val;
	return health;
}




