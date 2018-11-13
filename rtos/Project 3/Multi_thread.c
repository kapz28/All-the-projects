#include <lpc17xx.h>
#include <stdio.h>
#include <cmsis_os.h>
#include "MPU9250.h"
#include "led.h"
#include "sensor_fusion.h"

//Define Mutexes
osMutexDef (Mutex1);
osMutexDef (Mutex2);
osMutexDef (Mutex3);

osMutexId Mutex1_id;
osMutexId Mutex2_id;
osMutexId Mutex3_id;
 




//Initializes IMU, reads and stores sensor readings
void readData(void const *arg) {
	//Init
	MPU9250_init(1,1);	
	while(1) {
		//Update values
		osMutexWait(Mutex1_id, osWaitForever);
		MPU9250_read_gyro();
		MPU9250_read_acc();
		//Check if Magnetometer reading is valid
		if ((MPU9250_st_value & (0x01<<4)))
			MPU9250_read_mag();
		osMutexRelease(Mutex2_id);
		osThreadYield();
	};
}
osThreadDef(readData, osPriorityNormal, 1, 0);

//Perform sensor fusion of IMU data
void sensorFusion(void const *arg) {
	//Init
	sensor_fusion_init();
	sensor_fusion_begin(2000); // Higher frequency slows down simulation
	while(1){
		//Calculate pitch, roll and yaw
		osMutexWait(Mutex2_id, osWaitForever);
		sensor_fusion_update(MPU9250_gyro_data[0],MPU9250_gyro_data[1],MPU9250_gyro_data[2],MPU9250_accel_data[0],MPU9250_accel_data[1],MPU9250_accel_data[2],MPU9250_mag_data[0],MPU9250_mag_data[1],MPU9250_mag_data[2]);
		osMutexRelease(Mutex3_id);
		osThreadYield();
	};
}
osThreadDef(sensorFusion, osPriorityNormal, 1, 0);

//Thread for sending data to UART
void sendData(void const *arg) {
	//Init
	

	while(1){
		float pitch, yaw, roll;
		osMutexWait(Mutex3_id, osWaitForever);
		pitch = sensor_fusion_getPitch();
		yaw = sensor_fusion_getYaw();
		roll = sensor_fusion_getRoll();
		//Send pitch, roll, yaw to UART
		printf("%f,%f,%f\n", roll, -pitch, yaw);
		osMutexRelease(Mutex1_id);
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
	
	//Intialize mutexes
	Mutex1_id = osMutexCreate(osMutex (Mutex1));
	Mutex2_id = osMutexCreate(osMutex (Mutex2));
	Mutex3_id = osMutexCreate(osMutex (Mutex3)); \
	
	//osMutexWait(Mutex2_id, osWaitForever);
	//osMutexWait(Mutex3_id, osWaitForever);
	
	//Initialize threads
	osThreadCreate(osThread(readData), NULL);
	osThreadCreate(osThread(sensorFusion), NULL);
	osThreadCreate(osThread(sendData), NULL);
	
	
}
