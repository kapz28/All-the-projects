#include <lpc17xx.h>
#include <stdio.h>
#include <cmsis_os.h>
#include "MPU9250.h"
#include "led.h"
#include "sensor_fusion.h"

osMutexDef(Mutex1);
osMutexId(Mutex1_id);

//Initializes IMU, reads and stores sensor readings
void readData(void const *arg) {
	//Init
	osMutexCreate(osMutex(Mutex1));
	MPU9250_init(1,1);
	MPU9250_st_value |= (0x01<<4); 
	while(1) {
		//Update values
		osMutexWait(Mutex1_id, osWaitForever);
		MPU9250_read_gyro();
		MPU9250_read_acc();
		MPU9250_read_mag();
		osMutexRelease(Mutex1_id);
		osThreadYield();
	};
}
osThreadDef(readData, osPriorityNormal, 1, 0);

//Perform sensor fusion of IMU data
void sensorFusion(void const *arg) {
	//Init
	osMutexCreate(osMutex(Mutex1));
	sensor_fusion_init();
	sensor_fusion_begin(1000); // Higher frequency slows down simulation
	while(1){
		//Calculate pitch, roll and yaw
		osMutexWait(Mutex1_id, osWaitForever);
		sensor_fusion_update(MPU9250_gyro_data[0],MPU9250_gyro_data[1],MPU9250_gyro_data[2],MPU9250_accel_data[0],MPU9250_accel_data[1],MPU9250_accel_data[2],MPU9250_mag_data[0],MPU9250_mag_data[1],MPU9250_mag_data[2]);
		osMutexRelease(Mutex1_id);
		osThreadYield();
	};
}
osThreadDef(sensorFusion, osPriorityNormal, 1, 0);

//Thread for sending data to UART
void sendData(void const *arg) {
	//Init
	osMutexCreate(osMutex(Mutex1));
	while(1){
		float pitch, yaw, roll;
		osMutexWait(Mutex1_id, osWaitForever);
		pitch = sensor_fusion_getPitch();
		yaw = sensor_fusion_getYaw();
		roll = sensor_fusion_getRoll();
		osMutexRelease(Mutex1_id);
		//Send pitch, roll, yaw to UART
		printf("%f,%f,%f\n", roll, -pitch, -yaw);
		osThreadYield();
	};
}	
osThreadDef(sendData, osPriorityNormal, 1, 0);

//Main
int main(void) {	
	
	printf("INITIALIZED\n");
	
	//Initialize RTOS
	osKernelInitialize();
	osKernelStart();
	
	//Initialize threads
	osThreadCreate(osThread(readData), NULL);
	osThreadCreate(osThread(sensorFusion), NULL);
	osThreadCreate(osThread(sendData), NULL);
	
	
}
