#include <lpc17xx.h>
#include <stdio.h>
#include <cmsis_os.h>
#include "led.h"
#include "GLCD.h"

//Bitmaps
#include "x.c"
#include "o.c"
#include "white.c"
#include "blackH.c"
#include "blackV.c"
#include "redP.c"

//Define Mutexes
osMutexDef (cursorMutex);
osMutexDef (boardMutex);
osMutexDef (turnMutex);
osMutexDef (stateMutex);
osMutexDef (scoreMutex);

osMutexId cursorMutexId;
osMutexId boardMutexId;
osMutexId turnMutexId;
osMutexId stateMutexId;
osMutexId scoreMutexId;
 
typedef struct {
	uint8_t gamesPlayed;
	uint8_t score[2];
	uint8_t turn; //one will mean computer's turn and zero will mean players turn
	uint8_t board[3][3];	
	uint8_t cursor[2];
	uint8_t state;
}gameData_t;

gameData_t game;

/*
void drawCursor(int x, int y) {
	GLCD_Bitmap(x+3, y+3, 50, 2, (unsigned char*)redH);
	GLCD_Bitmap(x+3, y+51, 50, 2, (unsigned char*)redH);
	GLCD_Bitmap(x+3, y+3, 2, 50, (unsigned char*)redV);
	GLCD_Bitmap(x+51, y+3, 2, 50, (unsigned char*)redV);
}*/

//Controls of player
void player(void const *arg) {

	//Init
	volatile int joystick;
	volatile int button;
	uint8_t turn;
	uint8_t state;
	
	while(1) {
		
		osMutexWait(turnMutexId, 0);
		turn = game.turn;
		osMutexRelease(turnMutexId);
		
		osMutexWait(stateMutexId, 0);
		state = game.state;
		osMutexRelease(stateMutexId);
		
		if((turn % 2) == 1 && (turn > 0) && state == 0){
			
			joystick = LPC_GPIO1->FIOPIN;
			
			osMutexWait(cursorMutexId, 0);
			if (!(joystick & 0x00800000)) {
				//Up
				if (game.cursor[1] > 0)
					game.cursor[1]--;
			}else if (!(joystick & 0x02000000)) {
				//Down
				if (game.cursor[1] < 2)
					game.cursor[1]++;
			}else if (!(joystick & 0x04000000)) {
				//Left
				if (game.cursor[0] > 0)
					game.cursor[0]--;
			}else if (!(joystick & 0x01000000)) {
				//Right
				if (game.cursor[0] < 2)
					game.cursor[0]++;
			}
			osMutexRelease(cursorMutexId);
			
			button = (LPC_GPIO2->FIOPIN & (0x01<<10));
			if (!button) {
				osMutexWait(boardMutexId, 0);
				if (!game.board[game.cursor[0]][game.cursor[1]]) {
					game.board[game.cursor[0]][game.cursor[1]] = 1;
					osMutexWait(turnMutexId, 0);
					game.turn++;
					osMutexRelease(turnMutexId);
					osDelay(100);
				}
				osMutexRelease(boardMutexId);
			}
			
			osDelay(1300);
			osThreadYield();
		}
	};
}
osThreadDef(player, osPriorityNormal, 1, 0);


//Controls of Board
//Update the graphics of the board
void board(void const *arg) {
	//Init
	GLCD_Clear(White);

	uint8_t turn;
	uint8_t state;
	
	while(1) {
		//Thread loop 
		osMutexWait(stateMutexId, 0);
		state = game.state;
		osMutexRelease(stateMutexId);
		
		printf("%d\n", state);
		
		if (!state) {
		
			osMutexWait(boardMutexId, 0);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					//if (i != game.cursor[0] || j != game.cursor[1]) {
						if (game.board[i][j] == 0) 
							GLCD_Bitmap(25+i*56, 49+j*56, 55, 55, (unsigned char*)whiteBitmap);
						else if (game.board[i][j] == 1)
							GLCD_Bitmap(32+i*56, 56+j*56, 40, 40, (unsigned char*)xBitmap);
						else if (game.board[i][j] == 2)
							GLCD_Bitmap(32+i*56, 56+j*56, 40, 40, (unsigned char*)oBitmap);
					//}
				}
			}
			osMutexRelease(boardMutexId);
			
			for (unsigned int i = 0; i < 2; i++) {
				GLCD_Bitmap(24, 104 + i*56, 168, 1, (unsigned char*)blackH);
				GLCD_Bitmap(80 + 56*i, 48, 1, 168, (unsigned char*)blackV);
			}
			
			//drawCursor(24, 48);
			
			osMutexWait(turnMutexId, 0);
			turn = game.turn;
			osMutexRelease(turnMutexId);
			
			if (turn%2 == 1) {
				osMutexWait(cursorMutexId, 0);
				//GLCD_Bitmap(32+game.cursor[0]*56, 56+game.cursor[1]*56, 40, 40, (unsigned char*)xBitmap);
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						GLCD_Bitmap(32+game.cursor[0]*56+i, 56+game.cursor[1]*56+j, 1, 1, (unsigned char*)redP);
					}
				}
				osMutexRelease(cursorMutexId);
			}

			if (turn % 2 == 1)
				GLCD_DisplayString(0, 0, 1, (unsigned char*)"Player's Turn!");
			else if (turn %2 == 0)
				GLCD_DisplayString(0, 0, 1, (unsigned char*)"Thinking...   ");
			
			
		}else{
			
			osMutexWait(boardMutexId, 0);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (i != game.cursor[0] || j != game.cursor[1]) {
						if (game.board[i][j] == 0) 
							GLCD_Bitmap(25+i*56, 49+j*56, 55, 55, (unsigned char*)whiteBitmap);
						else if (game.board[i][j] == 1)
							GLCD_Bitmap(32+i*56, 56+j*56, 40, 40, (unsigned char*)xBitmap);
						else if (game.board[i][j] == 2)
							GLCD_Bitmap(32+i*56, 56+j*56, 40, 40, (unsigned char*)oBitmap);
					}
				}
			}
			osMutexRelease(boardMutexId);
			
			for (unsigned int i = 0; i < 2; i++) {
				GLCD_Bitmap(24, 104 + i*56, 168, 1, (unsigned char*)blackH);
				GLCD_Bitmap(80 + 56*i, 48, 1, 168, (unsigned char*)blackV);
			}

			if (state == 1){
				GLCD_DisplayString(0, 0, 1, (unsigned char*)"Player Win!   ");
			}else if (state == 2){
				GLCD_DisplayString(0, 0, 1, (unsigned char*)"CPU Win!      ");
			}else if (state == 3){
				GLCD_DisplayString(0, 0, 1, (unsigned char*)"Draw!         ");
			}else if (state > 3){
				break;
			}	
		}
		osThreadYield();
	};
	
	GLCD_Clear(White);
	if (state == 4){
		GLCD_DisplayString(1, 0, 1, (unsigned char*)"Player Win!   ");
	}else if (state == 5){
		GLCD_DisplayString(1, 0, 1, (unsigned char*)"CPU Win!      ");
	}else if (state == 6){
		GLCD_DisplayString(1, 0, 1, (unsigned char*)"Draw!         ");
	}
	
}
osThreadDef(board, osPriorityNormal, 1, 0);



//Controls Computer
void computer(void const *arg) {
	//Init
	uint8_t turn;
	uint8_t state;
	
	//Thread loop
	while(1) {
		
		osMutexWait(turnMutexId, 0);
		turn = game.turn;
		osMutexRelease(turnMutexId);
		
		osMutexWait(stateMutexId, 0);
		state = game.state;
		osMutexRelease(stateMutexId);
		
		osDelay(1000);
		
		if((turn % 2) == 0 && (turn > 0) && state == 0){
			printf("COMPUTER WILL MOVE!\n");
			osDelay(10000);
			uint8_t count = 0;
			uint8_t block = 0;
			uint8_t done = 0;
			osMutexWait(boardMutexId, 0);
			//WIN
			//horizontal 
			for (int i = 0; i < 3; i++) {
						count = 0;
						block = 0;
						for (int j = 0; j < 3; j++) {
								if (game.board[i][j] == 2) 
									count++;
								if (game.board[i][j] == 1) 
									block++;
						}
						if (count == 2){
							for (int j = 0; j < 3; j++) {
								if (game.board[i][j] == 0 && done == 0) {
									game.board[i][j] = 2;
									done++;
								}
							}
						}
						if (block == 2){
							for (int j = 0; j < 3; j++) {
								if (game.board[i][j] == 0 && done == 0){ 
									game.board[i][j] = 2;
									done++;
								}
							}
						}						
			}
			count = 0;
			block = 0;
			//vertical
			for (int j = 0; j < 3; j++) {
						count = 0;
						block = 0; 
						for (int i = 0; i < 3; i++) {
								if (game.board[i][j] == 2) 
									count++;
								if (game.board[i][j] == 1) 
									block++;
								
						}
						if (count == 2){
							for (int i = 0; i < 3; i++) {
								if (game.board[i][j] == 0 && done == 0){ 
									game.board[i][j] = 2;
									done++;
								}
							}
						}
						if (block == 2){
							for (int i = 0; i < 3; i++) {
								if (game.board[i][j] == 0 && done == 0){ 
									game.board[i][j] = 2;
									done++;
								}
							}
						}												
			}
			//diagonal
			count = 0;
			block = 0;
			for (int i = 0; i < 3; i++) {
						if (game.board[i][i] == 2) 
							count++;
						if (game.board[i][i] == 1) 
							block++;
						if (count == 2){
							for (int j = 0; j < 3; j++) {
								if (game.board[j][j] == 0 && done == 0){ 
									game.board[j][j] = 2;
									done++;
								}
							}
						}
						if (block == 2){
							for (int j = 0; j < 3; j++) {
								if (game.board[j][j] == 0 && done == 0){ 
									game.board[j][j] = 2;
									done++;
								}
							}
						}									
			}
			count = 0;
			block = 0;
			int g = 0;
			for (int i = 0; i < 3; i++) {
						g = 2 - i;
						if (game.board[i][g] == 2) 
							count++;
						if (game.board[i][g] == 1) 
							block++;
						if (count == 2){
							for (int j = 0; j < 3; j++) {
								g = 2 - j;
								if (game.board[j][g] == 0 && done == 0){ 
									game.board[j][g] = 2;
									done++;
								}
							}
						}
						if (block == 2){
							for (int j = 0; j < 3; j++) {
								g = 2 - j;
								if (game.board[j][g] == 0 && done == 0){ 
									game.board[j][g] = 2;
									done++;
								}
							}
						}									
			}
			//MIDDLE
			if (game.board[1][1] == 0 && done == 0){ 
				game.board[1][1] = 2;
				done++;
			}
			//CORNERS
			g = 0;
			for (int i = 0; i < 3; i+=2) {
						g = 2 - i;
						if (game.board[i][g] == 0 && done == 0){ 
								game.board[i][g] = 2;
								done++;
						}
						if (game.board[i][i] == 0 && done == 0){ 
								game.board[i][i] = 2;
								done++;
						}
			}
			//EDGE
			if (game.board[0][1] == 0 && done == 0){ 
					game.board[0][1] = 2;
					done++;
			}
			if (game.board[1][0] == 0 && done == 0){ 
					game.board[1][0] = 2;
					done++;
			}
			if (game.board[1][2] == 0 && done == 0){ 
					game.board[1][2] = 2;
					done++;
			}
			if (game.board[2][1] == 0 && done == 0){ 
					game.board[2][1] = 2;
					done++;
			}
			osMutexRelease(boardMutexId);
			
			osMutexWait(turnMutexId, 0);
			game.turn++;
			osMutexRelease(turnMutexId);
			
		}
		osThreadYield();
	};
}	
osThreadDef(computer, osPriorityNormal, 1, 0);

//Calculation
//checks if board is full and/or 10 games have been played and then does according action
void gameManagement(void const *arg) {
	//Init
	uint8_t state = 0;
	uint8_t turn;
	
	while(1) {
		
		if (game.gamesPlayed == 9) {
			osMutexWait(scoreMutexId, 0);
			if (game.score[0] > game.score[1])
				state = 4;
			else if (game.score[0] < game.score[1])
				state = 5;
			else
				state = 6;
			osMutexRelease(scoreMutexId);
			osMutexWait(stateMutexId, 0);
			game.state = state;
			osMutexRelease(stateMutexId);
		}else{
			osMutexWait(stateMutexId, 0);
			state = game.state;
			osMutexRelease(stateMutexId);
		}
		
		if (!state) { 
			//SET LED according to games played 
			if (game.gamesPlayed){
				if (game.gamesPlayed < 3){
					for (int h = 0; h < game.gamesPlayed; h++) {
							LPC_GPIO1->FIOSET |= (0x01<<(28+h));
					}
			  }
				if (game.gamesPlayed >= 3){
					for (int h = 0; h < 4; h++) {
							LPC_GPIO1->FIOSET |= (0x01<<(28+h));
					}
					for (int h = 0; h < (game.gamesPlayed-3); h++) {
							LPC_GPIO2->FIOSET |= (0x01<<(2+h));
					}
			  }
			}			
			osMutexWait(boardMutexId, 0);

			//Check rows and columns for a win
			for (int i = 0; i < 3; i++) {
				if (game.board[i][0]==1 && game.board[i][1]==1 && game.board[i][2]==1) {
					state = 1;
					osMutexWait(scoreMutexId, 0);
					game.score[0]++;
					osMutexRelease(scoreMutexId);
				}else if (game.board[i][0]==2 && game.board[i][1]==2 && game.board[i][2]==2) {
					state = 2;
					osMutexWait(scoreMutexId, 0);
					game.score[1]++;
					osMutexRelease(scoreMutexId);
				}
				
				if (game.board[0][i]==1 && game.board[1][i]==1 && game.board[2][i]==1) {
					state = 1;
					osMutexWait(scoreMutexId, 0);
					game.score[0]++;
					osMutexRelease(scoreMutexId);
				}else if (game.board[0][i]==2 && game.board[1][i]==2 && game.board[2][i]==2) {
					state = 2;
					osMutexWait(scoreMutexId, 0);
					game.score[1]++;
					osMutexRelease(scoreMutexId);
				}
			} 
			
			//Check diagonals for a win
			if (game.board[0][0]==1 && game.board[1][1]==1 && game.board[2][2]==1) {
				state = 1;
				osMutexWait(scoreMutexId, 0);
				game.score[0]++;
				osMutexRelease(scoreMutexId);
			}else if (game.board[0][0]==2 && game.board[1][1]==2 && game.board[2][2]==2) {
				state = 2;
				osMutexWait(scoreMutexId, 0);
				game.score[1]++;
				osMutexRelease(scoreMutexId);
			}
			
			if (game.board[0][2]==1 && game.board[1][1]==1 && game.board[2][0]==1) {
				state = 1;
				osMutexWait(scoreMutexId, 0);
				game.score[0]++;
				osMutexRelease(scoreMutexId);
			}else if (game.board[0][2]==2 && game.board[1][1]==2 && game.board[2][0]==2) {
				state = 2;
				osMutexWait(scoreMutexId, 0);
				game.score[1]++;
				osMutexRelease(scoreMutexId);
			}
			osMutexRelease(boardMutexId);
		
			osMutexWait(turnMutexId, 0);
			turn = game.turn;
			osMutexRelease(turnMutexId);
		
			if (turn == 10) 
				state = 3;
		
			if (state) {
				osMutexWait(stateMutexId, 0);
				game.state = state;
				osMutexRelease(stateMutexId);
				game.gamesPlayed++;
				osThreadYield();
				osDelay(20000);
				
				osMutexWait(boardMutexId, 0);
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						game.board[i][j] = 0;
					}
				}
				osMutexRelease(boardMutexId);
				
				osMutexWait(turnMutexId, 0);
				game.turn = 1;
				osMutexRelease(turnMutexId);
				
				osMutexWait(stateMutexId, 0);
				game.state = 0;
				osMutexRelease(stateMutexId);
			}
		}
		
		
		osThreadYield();
	};
}	
osThreadDef(gameManagement, osPriorityNormal, 1, 0);

//Main
int main(void) {	
	
	printf("INITIALIZED\n");

	
	//Initialize RTOS
	osKernelInitialize();
	osKernelStart();
	
	//Create and intialize memebers in struct 
	game.gamesPlayed = 0;
	game.score[0] = 0;
	game.score[1] = 0;
	game.turn = 0;
	for (int i = 0; i < 3; i++) 
		for (int j = 0; j < 3; j++) 
			game.board[i][j] = 0;
		
	game.cursor[0] = 0;
	game.cursor[1] = 0;
	game.state = 0;
	
	//Intialize LED
	LPC_GPIO1->FIODIR |= 0xB0000000;
	LPC_GPIO2->FIODIR |= 0x0000007C;
	LPC_GPIO1->FIOSET &= 0;
	
	//Intialize mutexes
	cursorMutexId = osMutexCreate(osMutex (cursorMutex));
	boardMutexId = osMutexCreate(osMutex (boardMutex));
	turnMutexId = osMutexCreate(osMutex (turnMutex)); 
	stateMutexId = osMutexCreate(osMutex (stateMutex)); 
	scoreMutexId = osMutexCreate(osMutex (scoreMutex)); 
	
	//Init
	GLCD_Init();
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(0, 0, 1, (unsigned char*)"TIC-TAC-TOE");
	GLCD_DisplayString(11, 0, 0, (unsigned char*)"Move with the joystick");
	GLCD_DisplayString(12, 0, 0, (unsigned char*)"Place X with the button");
	GLCD_DisplayString(13, 0, 0, (unsigned char*)"Best out of 8");
	GLCD_DisplayString(6, 0, 1, (unsigned char*)"Pushbutton to start!");
	
	
	//wait for pushbutton from user 
	int r1 = 0;
	//LPC_GPIO1->FIODIR |= 0xB0000000;
	//LPC_GPIO1->FIOSET &= 0;
	//int state = 0;
	//r1 = (LPC_GPIO2->FIOPIN & (0x01<<10));	
	do {
			r1 = (LPC_GPIO2->FIOPIN & (0x01<<10));
	} while(r1);
	printf("pushbutton pushed\n");
	
	osDelay(3000);
	game.turn++;

	//osDelay(10000);
	//Initialize threads
	osThreadCreate(osThread(player), NULL);
	osThreadCreate(osThread(board), NULL);
	osThreadCreate(osThread(computer), NULL);
	osThreadCreate(osThread(gameManagement), NULL);	
}
