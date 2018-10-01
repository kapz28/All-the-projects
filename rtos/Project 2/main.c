#include <lpc17xx.h>
#include <stdio.h>
#include "GLCD.h"
#include <cmsis_os.h>

//Delay Function for sensor reading/LCD refresh
void delay(unsigned int ms) {
	for (int i = 0; i < ms; i++) {}
}


//LED Test: Displays 123 (dec)
/*
int main(void) {

	//
	LPC_GPIO1->FIODIR |= 0xB0000000;
	LPC_GPIO2->FIODIR |= 0x0000007C;
	
	LPC_GPIO1->FIOSET |= 0xA0000000;
	LPC_GPIO2->FIOSET |= 0x0000006C;
	
	while(1);
}
*/



//Joystick sensing
/*
int main(void) {
	
	printf("ALL GOOD");

	
	while(1){
		int r1 = LPC_GPIO1->FIOPIN;
		
		//printf("%d\n", r1 &= 0x00100000);
		
		int up = LPC_GPIO1->FIOPIN;
		int down = LPC_GPIO1->FIOPIN;
		int left = LPC_GPIO1->FIOPIN;
		int right = LPC_GPIO1->FIOPIN;
		int button = LPC_GPIO1->FIOPIN;
		
		
		if (!(up &= 0x00800000)) {
			printf("UP ");
		}else if (!(down &= 0x02000000)) {
			printf("DOWN ");
		}else if (!(left &= 0x04000000)) {
			printf("LEFT ");
		}else if (!(right &= 0x01000000)) {
			printf("RIGHT ");
		}else{
			printf("NO DIR ");
		}
		
		if (!(button &= 0x00100000)) {
			printf("BUTTON: PRESSED\n");
		}else{
			printf("BUTTON: NOT PRESSED\n");
		}
		delay(1000000);
	}
	
}*/

// print outs the value of the potentiometer
/*
int main(void) {
	
	
	
	printf("ALL GOOD");
	LPC_PINCON->PINSEL1 &= ~(0x03<<18);
	LPC_PINCON->PINSEL1 |= (0x01<<18);
	LPC_SC->PCONP |= (0x01<<12);
	LPC_ADC->ADCR = (1 << 2) | (4 << 8) | (1 << 21);

	
	int status = 0;
	
	while(1) {

		LPC_ADC->ADCR |= (0x01<<24);
		
		do {
			status = LPC_ADC->ADGDR;
			//printf("LOOPING\n");
		}while(!(status &= (0x01<<31)));
	
		int val = 0;
		
		val = LPC_ADC->ADGDR;
		printf("%d\n", (val &= (0x0FFF<<4)>>4));
		delay(1000000);
	}	
}*/


//LCD display
/*
int main(void) {	
	printf("ALL GOOD");
	
	GLCD_Init();
	GLCD_Clear(Blue);
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Blue);
	unsigned char p2[14] = "Hello, World!";
	//GLCD_DisplayChar(4,4,'1','F');
	GLCD_DisplayString(4,4,1,p2);
	
}*/


void potentiometer(void const *arg) {
	
	LPC_PINCON->PINSEL1 &= ~(0x03<<18);
	LPC_PINCON->PINSEL1 |= (0x01<<18);
	LPC_SC->PCONP |= (0x01<<12);
	LPC_ADC->ADCR = (1 << 2) | (4 << 8) | (1 << 21);
	
	int status = 0;
	int val = 0;
	
	while(1) {
		LPC_ADC->ADCR |= (0x01<<24);
		
		do {
			status = LPC_ADC->ADGDR;
		} while(!(status &= (0x01<<31)));
	
		val = LPC_ADC->ADGDR;
		printf("%d\n", (val &= (0x0FFF<<4)>>4));
		
		osThreadYield();
	}	
}

osThreadDef(potentiometer, osPriorityNormal, 1, 0);

void joystick(void const *arg) {
	
	GLCD_Init();
	GLCD_Clear(Blue);
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Blue);
	unsigned char up_s[] = "UP";
	unsigned char down_s[] = "DOWN";
	unsigned char left_s[] = "LEFT";
	unsigned char right_s[] = "RIGHT";
	unsigned char neutral_s[] = "NO DIR";
	unsigned char p[] = "PRESSED";
	unsigned char np[] = "NOT PRESSED";
	

	while(1){
		GLCD_ClearLn(4, 1);
		GLCD_ClearLn(5, 1);
		
		
		int up = LPC_GPIO1->FIOPIN;
		int down = LPC_GPIO1->FIOPIN;
		int left = LPC_GPIO1->FIOPIN;
		int right = LPC_GPIO1->FIOPIN;
		int button = LPC_GPIO1->FIOPIN;
		
		
		if (!(up &= 0x00800000)) {
			GLCD_DisplayString(4,2,1,up_s);
		}else if (!(down &= 0x02000000)) {
			GLCD_DisplayString(4,2,1,down_s);
		}else if (!(left &= 0x04000000)) {
			GLCD_DisplayString(4,2,1,left_s);
		}else if (!(right &= 0x01000000)) {
			GLCD_DisplayString(4,2,1,right_s);
		}else{
			GLCD_DisplayString(4,2,1,neutral_s);
		}
		
		if (!(button &= 0x00100000)) {
			GLCD_DisplayString(5,2,1,p);
		}else{
			GLCD_DisplayString(5,2,1,np);
		}
		
		delay(10000000);
		
		osThreadYield();
	}
	
}

osThreadDef(joystick, osPriorityNormal, 1, 0);

void LED(void const *arg) {
	int r1 = 0;
	int state = 0;
	LPC_GPIO1->FIODIR |= 0xB0000000;
	LPC_GPIO1->FIOSET &= 0;
	
	while(1){
		r1 = (LPC_GPIO2->FIOPIN & (0x01<<10));
		
		if(!r1){
			if (!state){
				LPC_GPIO1->FIOSET |= (0x01<<28);
				state = 1;
			}else{
				LPC_GPIO1->FIOCLR |= (0x01<<28);
				state = 0;
			}
			delay(3000000);
		}
		
		osThreadYield();
	};
}	

osThreadDef(LED, osPriorityNormal, 1, 0);

//Threads Program
/*
int main(void) {	
	
	printf("INITIALIZED");
	
	osKernelInitialize();
	osKernelStart();
	
	osThreadCreate(osThread(potentiometer), NULL);
	osThreadCreate(osThread(joystick), NULL);
	osThreadCreate(osThread(LED), NULL);
	
	
}*/
