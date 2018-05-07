//kapilan satkunanathan and yuanpei gao
#ifndef MIDDLEEND_HPP
#define MIDDLEEND_HPP
#include <string>
class Game{
public:

    int *position_array;
    char *element_array;
	
	// CONSTRUCTOR/DESTRUCTOR
    // Create a new empty DoublyLinkedList.
    Game();
    Game(int boardsize,char mode, int*** pos_point,char*** ele_point) ;
    // Destroy this Game, freeing all dynamically allocated memory.
    ~Game();

	// ACCESSORS
    // Returns the size of board.
    unsigned int size() const;
    // Returns if end game conditions are met
    bool end_game_cond() const;
	//find number of items left on the board    
    int get_num_items();
	//get num_items    
    void update_num_items(int new_num);
	//game board
    void print();
    //updates board after move
    bool update_board(char move_player_one,char move_player_two) const;

    
private:
	// MEMBER VARIABLES
	int num_items=0;

    // The number of elements in the list.
    unsigned int board_size;
   
};
class Player{
public:
	//Create a new player aka Constructor
	Player();
	
	//Destructor
	~Player();
	int points;
	int position;
	
	// get health 
	int get_health();
	
	// modify health 
	int health_modify(int val);

	int health =100;
};
#endif 
