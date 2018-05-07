//Kapilan Satkunanathan
#include <iostream>
#include <string>
#include "middleend.hpp"
#include <sstream>

using namespace std;

int main()
{
  int i;
  cout << "Please enter an integer value for board size between 10 and 30:"<<endl;
  cin >> i;
  while(i<10||i>30){
  	cout << "in-valid entry:Please enter an integer value for board size between 10 and 30:"<<endl;
  	cin >> i;	
  }
  char j;
  cout << "Please enter a letter for mode to play currently available mode  PVP char is P"<<endl;
  cin >> j;
  while(j != 'P'){
  	cout << "in-valid entry:Please enter a letter for mode to play currently available mode  PVP char is P"<<endl;
  	cin >> j;	
  }
  int ***a;
  char ***b;
  Game board(i,j, a,b);
  board.print();
  char moveone, movetwo;
  while(!(board.end_game_cond())){
	  cout << "Please enter a letter for mode to play currently available mode  PVP char is P"<<endl;
	  cin >> moveone;
	  while(moveone != 'P'){
	  	cout << "in-valid entry:Please enter a letter for mode to play currently available mode  PVP char is P"<<endl;
	  	cin >> moveone;	
	  }
	  cout << "Please enter a letter for mode to play currently available mode  PVP char is P"<<endl;
	  cin >> movetwo;
	  while(movetwo != 'W'){
	  cout << "in-valid entry:Please enter a letter for mode to play currently available mode  PVP char is P"<<endl;
	  cin >> movetwo;	
	  }
	board.print();
	}
  
  system("pause");
  system("pause");

}
