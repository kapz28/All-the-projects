/*=============================================================================
  *
	*  TritonCal.c - DSP algorithm related to calibrating the unit
	*
=============================================================================*/
#include "TritonCal.h"
#include "DSPUtils.h" //max,min
#include "cat3626.h"
#include "TritonParams.h" //GYROSENS
#include "TritonBuffers.h"//DDR_Buffers
#include "cal.h"


#define TOLERANCE 1
#define SAMPLESIZE  228
#define COLLECT_SIZE 500
#define STEADY_LIMIT 45
#define ZERO_LIMIT 3000
#define LOW_LIMIT 6000
#define HIGH_LIMIT 15000
#define SAMPLESBY6 38
#define IDEALRANGE 16384


void tritoncal_cal_newwin(struct TritonCalInfo *calinfo,int Nwin)
{
	for (int idx = DDR_SIZE - Nwin; idx < DDR_SIZE; idx++)
	{
		Ox_DDR_Buffer[idx] = Ox_DDR_Buffer[idx] + calinfo->gyro_offset.x;
		Oy_DDR_Buffer[idx] = Oy_DDR_Buffer[idx] + calinfo->gyro_offset.y;
		Oz_DDR_Buffer[idx] = Oz_DDR_Buffer[idx] + calinfo->gyro_offset.z;
	}
}

void tritoncal_calGyro(struct TritonCalInfo *calinfo, struct TritonFeatures *New)
{
	int maxgyro_range = max( max(New->Ox_DDR_stats.range, New->Oy_DDR_stats.range),
			New->Oz_DDR_stats.range);

	//DEBUG_PRINTF(("gyro range: %d\n",maxgyro_range));
	bool nomotion = maxgyro_range < 5; //5LSB

	if (nomotion)
	{
		//divide by two the mean, so is somewhat filtered change
		calinfo->gyro_offset.x = calinfo->gyro_offset.x - (New->Ox_DDR_stats.mean * 0.5f);
		calinfo->gyro_offset.y = calinfo->gyro_offset.y - (New->Oy_DDR_stats.mean * 0.5f);
		calinfo->gyro_offset.z = calinfo->gyro_offset.z - (New->Oz_DDR_stats.mean * 0.5f);

	//	DEBUG_PRINTF(("--- new offset: [%d %d %d]\n",calinfo->gyro_offset.x,calinfo->gyro_offset.y,calinfo->gyro_offset.z));
	}
}

void tritoncal_calAcc(struct TritonCalInfo *calinfo, struct TritonFeatures *New)
{
	int maxacc_range = max( max(New->Ax_DDR_stats.range, New->Ay_DDR_stats.range),
			New->Az_DDR_stats.range);

	//DEBUG_PRINTF(("acc range: %d\n",maxgyro_range));
	bool nomotion = maxacc_range < 5; //5LSB

	if (nomotion)
	{
		//divide by two the mean, so is somewhat filtered change
		calinfo->acc_offset.x = calinfo->acc_offset.x - (New->Ax_DDR_stats.mean * 0.5f);
		calinfo->acc_offset.y = calinfo->acc_offset.y - (New->Ay_DDR_stats.mean * 0.5f);
		calinfo->acc_offset.z = calinfo->acc_offset.z - (New->Az_DDR_stats.mean * 0.5f);

	//	DEBUG_PRINTF(("--- new offset: [%d %d %d]\n",calinfo->gyro_offset.x,calinfo->gyro_offset.y,calinfo->gyro_offset.z));
	}
}

void tritoncal_caltestresults(){


	const char *linked_test_descriptions[3];
	linked_test_descriptions[0] = "Test1: checks if the offset math checks out";
	linked_test_descriptions[1] = "Test2: checks if the colors of the unit works properly and detects when it is steady";
	linked_test_descriptions[2]= "Test3: checks if trigger works properly";

	bool linked_test_results[12];
    linked_test_results[0] = tritoncal_sixcal_testoffsetmath();
    linked_test_results[1] = tritoncal_sixcal_testcolorstate();
    linked_test_results[2] = tritoncal_sixcal_testtriggerworks();



    log_printf("\n ************************************** TEST RESULTS ****************************************************\n");
    log_printf("\n ******************************************************************************************************* \n");

    for (int i = 0; i < 3; ++i) {
    	log_printf(" \n %s \n ",linked_test_descriptions[i]);
    	log_printf("\n TEST number : #%d ", i);
    	if(linked_test_results[i]){
    		log_printf(" RESULT IS: PASS \n");
    	}
    	else if(!(linked_test_results[i])){
    		log_printf(" RESULT IS: FAIL \n");
    	}

	}

	log_printf("\n ************************************** TEST RESULTS END ****************************************************\n");
    log_printf("\n *********************************************************************************************************** \n");

}
int tritoncal_sixcal(struct vector *acc_scale_vec,struct vector *acc_offset_vec,struct vector *acc_mean_vec, struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo){

	/*
    int SAMPLESIZE = SDR_RAW_WIN_SIZE/2;
	#define COLLECT_SIZE 500
	#define STEADY_LIMIT 45
	#define ZERO_LIMIT 55
	#define LOW_LIMIT 15
	#define HIGH_LIMIT 85
	int16_t SAMPLESBY6 = (samplesize/6);
	 */

	/* declare outside the function as intializors
	calinfo->trigger = 1;
	calinfo->trip =0;
	calinfo->count_run = 0;
	calinfo->calinfo->isit6yet = 0;
	*/

	if ((calinfo->trigger)){
		cat3626_setCyan();
	}
	if (!(calinfo->trip)){
		tritoncal_calcolorset(acc_stddev_vec, calinfo);
	}
	else if(calinfo->trip){
		tritoncal_calsteadycheck(acc_mean_vec, acc_stddev_vec, calinfo);
	}
	if(calinfo->isit6yet >= 6){
		tritoncal_caloffset(acc_scale_vec,acc_offset_vec, calinfo);
	}
	//KAPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
	//END
	return 1;
}

int tritoncal_caloffset(struct vector *acc_scale_vec,struct vector *acc_offset_vec, struct TritonCalInfo *calinfo){
	log_printf("\n SIX POINTS RECORDED \n");

	acc_offset_vec->x = ((calinfo->x_axis_extr.x)+(calinfo->x_axis_extr.y))/(2.0);
	acc_offset_vec->y = ((calinfo->y_axis_extr.x)+(calinfo->y_axis_extr.y))/(2.0);
	acc_offset_vec->z = ((calinfo->z_axis_extr.x)+(calinfo->z_axis_extr.y))/(2.0);
	acc_scale_vec->x = (IDEALRANGE)/((calinfo->x_axis_extr.x - acc_offset_vec->x) - (calinfo->x_axis_extr.y - acc_offset_vec->x));
	acc_scale_vec->y = (IDEALRANGE)/((calinfo->y_axis_extr.x - acc_offset_vec->y) - (calinfo->y_axis_extr.y - acc_offset_vec->y));
	acc_scale_vec->z = (IDEALRANGE)/((calinfo->z_axis_extr.x - acc_offset_vec->z) - (calinfo->z_axis_extr.y - acc_offset_vec->z));
	log_printf("\n cali pos is x- %f y- %f z- %f \n", (calinfo->x_axis_extr.x), (calinfo->y_axis_extr.x), (calinfo->z_axis_extr.x));
	log_printf("\n cali neg is x- %f y- %f z- %f \n", (calinfo->x_axis_extr.y), (calinfo->y_axis_extr.y), (calinfo->z_axis_extr.y));
	log_printf("\n cali off is x- %f y- %f z- %f \n", (acc_offset_vec->x), (acc_offset_vec->y), (acc_offset_vec->z));
	log_printf("\n cali scale is x- %f y- %f z- %f \n", (acc_scale_vec->x), (acc_scale_vec->y), (acc_scale_vec->z));
	//log_printf("\n cali check equal - %f \n", ((calinfo->y_axis_extr.x - acc_offset_vec->y) - (calinfo->y_axis_extr.y - acc_offset_vec->y)));
	bool applied = cal_sixcalsetoffset(acc_offset_vec);
	if (applied){
		log_printf("\n OFFSET APPLIED \n");
	}
	else{
		log_printf("\n OFFSET DIDN'T APPLIED \n");
	}
	log_printf("\n EXITING CALI MODE \n");
	//CALIBRATION_MODE = 0;
	calinfo->collection_table.point1 = 0;
	calinfo->collection_table.point2 = 0;
	calinfo->collection_table.point3 = 0;
	calinfo->collection_table.point4 = 0;
	calinfo->collection_table.point5 = 0;
	calinfo->collection_table.point6 = 0;
	calinfo->isit6yet = 0;

	return 1;
}
bool tritoncal_calcolorset(struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo){
	if (((acc_stddev_vec->x) <= STEADY_LIMIT) && ((acc_stddev_vec->y) <= STEADY_LIMIT) && ((acc_stddev_vec->z) <= STEADY_LIMIT)){
		cat3626_setPurple();
		calinfo->count_run = calinfo->count_run + 38;
		calinfo->trigger = 0;
	}
	else if (((acc_stddev_vec->x) >= STEADY_LIMIT) && ((acc_stddev_vec->y) >= STEADY_LIMIT) && ((acc_stddev_vec->z) >= STEADY_LIMIT) ){
		calinfo->trigger = 1;
		calinfo->count_run = 0;
		cat3626_setCyan();
	}
	if ((calinfo->count_run > COLLECT_SIZE) && (!(calinfo->trigger))){
		cat3626_setGreen();
		calinfo->count_run = 0;
		calinfo->trip = 1;
	}
	return 1;
}
int tritoncal_caltriggerstate(struct vector *acc_mean_vec, struct TritonCalInfo *calinfo){
		if((calinfo->trigger_pressed)){
			if(((HIGH_LIMIT > (acc_mean_vec->z)) && ((acc_mean_vec->z) > LOW_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->y)) && ((acc_mean_vec->y) > -ZERO_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->x)) && ((acc_mean_vec->x) > -ZERO_LIMIT))){
				if(!(calinfo->collection_table.point5)){
					calinfo->z_axis_extr.x = (acc_mean_vec->z);
					calinfo->isit6yet = (calinfo->isit6yet) + 1;
					calinfo->collection_table.point5 =1;
					calinfo->trigger_pressed = 0;
				}
				else if((calinfo->collection_table.point5)){
					calinfo->trigger_pressed = 0;
					log_printf("\n THIS POINT HAS ALREADY BEEN RECORDED \n");
				}
			}
			else if(((-HIGH_LIMIT < (acc_mean_vec->z)) && ((acc_mean_vec->z) < -LOW_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->y)) && ((acc_mean_vec->y) > -ZERO_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->x)) && ((acc_mean_vec->x) > -ZERO_LIMIT))){
				if(!(calinfo->collection_table.point6)){
					calinfo->z_axis_extr.y = (acc_mean_vec->z);
					calinfo->isit6yet = (calinfo->isit6yet) + 1;
					calinfo->collection_table.point6 = 1;
					calinfo->trigger_pressed = 0;
				}
				else if((calinfo->collection_table.point6)){
					(calinfo->trigger_pressed) = 0;
					log_printf("\n THIS POINT HAS ALREADY BEEN RECORDED \n");
				}
			}
			else if(((HIGH_LIMIT > (acc_mean_vec->y)) && ((acc_mean_vec->y) > LOW_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->z)) && ((acc_mean_vec->z) > -ZERO_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->x)) && ((acc_mean_vec->x) > -ZERO_LIMIT))){
				if(!(calinfo->collection_table.point3)){
					calinfo->y_axis_extr.x = (acc_mean_vec->y);
					calinfo->collection_table.point3 =1;
					calinfo->isit6yet = (calinfo->isit6yet) + 1;
					calinfo->trigger_pressed = 0;
				}
				else if((calinfo->collection_table.point3)){
					calinfo->trigger_pressed = 0;
					log_printf("\n THIS POINT HAS ALREADY BEEN RECORDED \n");
				}
			}
			else if(((-HIGH_LIMIT < (acc_mean_vec->y)) && ((acc_mean_vec->y) < -LOW_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->z)) && ((acc_mean_vec->z) > -ZERO_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->x)) && ((acc_mean_vec->x) > -ZERO_LIMIT))){
				if(!(calinfo->collection_table.point4)){
					calinfo->y_axis_extr.y = (acc_mean_vec->y);
					calinfo->collection_table.point4 =1;
					calinfo->isit6yet = (calinfo->isit6yet) + 1;
					calinfo->trigger_pressed = 0;
				}
				else if((calinfo->collection_table.point4)){
					(calinfo->trigger_pressed) = 0;
					log_printf("\n THIS POINT HAS ALREADY BEEN RECORDED \n");
				}
			}
			else if(((HIGH_LIMIT > (acc_mean_vec->x)) && ((acc_mean_vec->x) > LOW_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->z)) && ((acc_mean_vec->z) > -ZERO_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->y)) && ((acc_mean_vec->y) > -ZERO_LIMIT))){
				if(!(calinfo->collection_table.point1)){
					calinfo->x_axis_extr.x = (acc_mean_vec->x);
					calinfo->collection_table.point1 = 1;
					calinfo->isit6yet = (calinfo->isit6yet) + 1;
					calinfo->trigger_pressed = 0;
				}
				else if((calinfo->collection_table.point1)){
					calinfo->trigger_pressed = 0;
					log_printf("\n THIS POINT HAS ALREADY BEEN RECORDED \n");
				}
			}
			else if(((-HIGH_LIMIT < (acc_mean_vec->x)) && ((acc_mean_vec->x) < -LOW_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->z)) && ((acc_mean_vec->z) > -ZERO_LIMIT)) && ((ZERO_LIMIT > (acc_mean_vec->y)) && ((acc_mean_vec->y) > -ZERO_LIMIT))){
				if(!(calinfo->collection_table.point2)){
					calinfo->x_axis_extr.y = (acc_mean_vec->x);
					calinfo->isit6yet = (calinfo->isit6yet) + 1;
					calinfo->collection_table.point2 =1;
					calinfo->trigger_pressed = 0;
				}
				else if((calinfo->collection_table.point2)){
					calinfo->trigger_pressed = 0;
					log_printf("\n THIS POINT HAS ALREADY BEEN RECORDED \n");
				}
			}
		}
		return 1;
}

int tritoncal_calsteadycheck(struct vector *acc_mean_vec, struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo){

	log_printf("\n TRIP HAPPENED \n");
	if (((acc_stddev_vec->x) >= STEADY_LIMIT) || ((acc_stddev_vec->y) >= STEADY_LIMIT) || ((acc_stddev_vec->z) >= STEADY_LIMIT)){
		calinfo->trip = 0;
		calinfo->count_run = 0;
		calinfo->trigger_pressed = 0;
		log_printf("\n TRIGGER RESET \n");
	}
	else if (((acc_stddev_vec->x) < STEADY_LIMIT) && ((acc_stddev_vec->y) < STEADY_LIMIT) && ((acc_stddev_vec->z) < STEADY_LIMIT)){
		log_printf("\n POINT IS: x- %f y -%f z -%f \n", (acc_mean_vec->x), (acc_mean_vec->y), (acc_mean_vec->z));
		log_printf("\n READY TO BE TRIGGERED \n");
		tritoncal_caltriggerstate(acc_mean_vec, calinfo);
		log_printf("\n NUMBER OF POINTS RECORDED %f \n", calinfo->isit6yet);
	}

	return 1;

}
/* Initializor */
bool tritoncal_sixinitialize(struct vector *acc_scale_vec, struct vector *acc_offset_vec,struct vector *acc_mean_vec, struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo){
	//calinfo->collection_table.point6
	//calinfo->collection_table.point6
	calinfo->collection_table.point1 = 0;
	calinfo->collection_table.point2 = 0;
	calinfo->collection_table.point3 = 0;
	calinfo->collection_table.point4 = 0;
	calinfo->collection_table.point5 = 0;
	calinfo->collection_table.point6 = 0;
	calinfo->trigger = 1;
	calinfo->trip = 0;
	calinfo->count_run = 0;
	calinfo->isit6yet = 0;
	calinfo->trigger_pressed = 0;
	acc_mean_vec->x = 0;
	acc_mean_vec->y = 0;
	acc_mean_vec->z = 0;
	acc_offset_vec->x = 0;
	acc_offset_vec->y = 0;
	acc_offset_vec->z = 0;
	acc_scale_vec->x = 1;
	acc_scale_vec->y = 1;
	acc_scale_vec->z = 1;
	acc_stddev_vec->x = 0;
	acc_stddev_vec->y = 0;
	acc_stddev_vec->z = 0;
	calinfo->x_axis_extr.x = 0;
	calinfo->x_axis_extr.y = 0;
	calinfo->y_axis_extr.x = 0;
	calinfo->y_axis_extr.y = 0;
	calinfo->z_axis_extr.x = 0;
	calinfo->z_axis_extr.y = 0;

	return 1;

}
// setters and checkers

bool tritoncal_calsetter_sixcal(struct vector *acc_offset_vec ,struct vector *acc_mean_vec , struct vector *acc_stddev_vec , struct TritonCalInfo *calinfo,float averagex , float averagey , float averagez , float std_deviationx, float std_deviationy, float std_deviationz, float offset_x , float offset_y, float offset_z, int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6){

	acc_mean_vec->x = averagex;
	acc_mean_vec->y = averagey;
	acc_mean_vec->z = averagez;
	acc_offset_vec->x = offset_x;
	acc_offset_vec->y = offset_y;
	acc_offset_vec->z = offset_z;
	acc_stddev_vec->x = std_deviationx;
	acc_stddev_vec->y = std_deviationy;
	acc_stddev_vec->z = std_deviationz;
	calinfo->x_axis_extr.x = x_xtr_1;
	calinfo->x_axis_extr.y = x_xtr_2;
	calinfo->y_axis_extr.x = y_xtr_1;
	calinfo->y_axis_extr.y = y_xtr_2;
	calinfo->z_axis_extr.x = z_xtr_1;
	calinfo->z_axis_extr.y = z_xtr_2;
	calinfo->isit6yet = isit6yet ;
	calinfo->trigger = trigger;
	calinfo->count_run = count_run;
	calinfo->trip = trip ;
	calinfo->trigger_pressed = trigger_pressed;
	calinfo->collection_table.point1 = point_1;
	calinfo->collection_table.point2 = point_2;
	calinfo->collection_table.point3 = point_3;
	calinfo->collection_table.point4 = point_4;
	calinfo->collection_table.point5 = point_5;
	calinfo->collection_table.point6 = point_6;

	return 1;
}
bool tritoncal_calchecker_sixcal(struct vector *acc_offset_vec ,struct vector *acc_mean_vec , struct vector *acc_stddev_vec , struct TritonCalInfo *calinfo, float offset_x , float offset_y, float offset_z, int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6){
	if(acc_offset_vec->x == offset_x && acc_offset_vec->y == offset_y && acc_offset_vec->z == offset_z){
		if(calinfo->x_axis_extr.x == x_xtr_1 && calinfo->x_axis_extr.x == x_xtr_2 && calinfo->y_axis_extr.x == y_xtr_1 && calinfo->y_axis_extr.y == y_xtr_2 && calinfo->z_axis_extr.x == z_xtr_1 && calinfo->z_axis_extr.y == z_xtr_2){
			 if(calinfo->isit6yet == isit6yet && calinfo->trigger == trigger && calinfo->count_run == count_run && calinfo->trip == trip && calinfo->trigger_pressed == trigger_pressed){
				if(calinfo->collection_table.point1 == point_1 && calinfo->collection_table.point2 == point_2 && calinfo->collection_table.point3 == point_3 && calinfo->collection_table.point4 == point_4 && calinfo->collection_table.point5 == point_5 && calinfo->collection_table.point6 == point_6){
					return 1;
				}
			 }
		}
	}
	return 0;
}
bool tritoncal_calsetter_caloffset(struct vector *acc_scale_vec,struct vector *acc_offset_vec, struct TritonCalInfo *calinfo , float offset_x , float offset_y, float offset_z, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, float scale_x , float scale_y, float scale_z){

	acc_scale_vec->x = scale_x;
	acc_scale_vec->y = scale_y;
	acc_scale_vec->z = scale_z;
	acc_offset_vec->x = offset_x;
	acc_offset_vec->y = offset_y;
	acc_offset_vec->z = offset_z;
	calinfo->x_axis_extr.x = x_xtr_1;
	calinfo->x_axis_extr.y = x_xtr_2;
	calinfo->y_axis_extr.x = y_xtr_1;
	calinfo->y_axis_extr.y = y_xtr_2;
	calinfo->z_axis_extr.x = z_xtr_1;
	calinfo->z_axis_extr.y = z_xtr_2;

	return 1;
}
bool tritoncal_calchecker_caloffset(struct vector *acc_scale_vec,struct vector *acc_offset_vec, float offset_x , float offset_y, float offset_z, float scale_x , float scale_y, float scale_z){
	if(acc_scale_vec->x == scale_x && acc_scale_vec->y == scale_y && acc_scale_vec->z == scale_z){
		if(acc_offset_vec->x == offset_x && acc_offset_vec->y == offset_y && acc_offset_vec->z == offset_z){
				return 1;
		}
	}
	return 0;
}
bool tritoncal_calsetter_calcolorset(struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo, float std_deviationx, float std_deviationy, float std_deviationz,  bool trigger, int count_run, int trip){

	acc_stddev_vec->x = std_deviationx;
	acc_stddev_vec->y = std_deviationy;
	acc_stddev_vec->z = std_deviationz;
	calinfo->trigger = trigger;
	calinfo->count_run = count_run;
	calinfo->trip = trip;

	return 1;
}
bool tritoncal_calchecker_calcolorset(struct TritonCalInfo *calinfo, bool trigger, int count_run, int trip){
	if( calinfo->trigger == trigger && calinfo->count_run == count_run && calinfo->trip == trip){
		return 1;
	 }

	return 0;
}
bool tritoncal_calsetter_calsteadycheck(struct vector *acc_mean_vec , struct vector *acc_stddev_vec , struct TritonCalInfo *calinfo,float averagex , float averagey , float averagez , float std_deviationx, float std_deviationy, float std_deviationz, int isit6yet , int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6){

	acc_mean_vec->x = averagex;
	acc_mean_vec->y = averagey;
	acc_mean_vec->z = averagez;
	acc_stddev_vec->x = std_deviationx;
	acc_stddev_vec->y = std_deviationy;
	acc_stddev_vec->z = std_deviationz;
	calinfo->x_axis_extr.x = x_xtr_1;
	calinfo->x_axis_extr.y = x_xtr_2;
	calinfo->y_axis_extr.x = y_xtr_1;
	calinfo->y_axis_extr.y = y_xtr_2;
	calinfo->z_axis_extr.x = z_xtr_1;
	calinfo->z_axis_extr.y = z_xtr_2;
	calinfo->isit6yet = isit6yet ;
	calinfo->trip = trip ;
	calinfo->trigger_pressed = trigger_pressed;
	calinfo->collection_table.point1 = point_1;
	calinfo->collection_table.point2 = point_2;
	calinfo->collection_table.point3 = point_3;
	calinfo->collection_table.point4 = point_4;
	calinfo->collection_table.point5 = point_5;
	calinfo->collection_table.point6 = point_6;

	return 1;
}
bool tritoncal_calchecker_calsteadycheck(struct TritonCalInfo *calinfo, int isit6yet , int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6){
/*
	log_printf("\n********* START COMPARE **********\n");
	log_printf("\n x_axis_extr.x %f , %f\n" ,calinfo->x_axis_extr.x, x_xtr_1);
	log_printf("\n x_axis_extr.y %f , %f\n" ,calinfo->x_axis_extr.y, x_xtr_2);
	log_printf("\n y_axis_extr.x %f , %f\n" ,calinfo->y_axis_extr.x, y_xtr_1);
	log_printf("\n y_axis_extr.y %f , %f\n" ,calinfo->y_axis_extr.y, y_xtr_2);
	log_printf("\n z_axis_extr.x %f , %f\n" ,calinfo->z_axis_extr.x, z_xtr_1);
	log_printf("\n z_axis_extr.y %f ,%f\n" ,calinfo->z_axis_extr.y, z_xtr_2);
	log_printf("\n isit6yet %d ,%d \n" , calinfo->isit6yet, isit6yet);
	log_printf("\n trip %d , %d \n" , calinfo->trip, trip);
	log_printf("\n trigger_pressed %d , %d\n" ,calinfo->trigger_pressed , trigger_pressed);
	log_printf("\n point1 %d , %d \n" ,calinfo->collection_table.point1 , point_1);
	log_printf("\n point2 %d , %d \n" ,calinfo->collection_table.point2, point_2);
	log_printf("\n point3 %d , %d \n" ,calinfo->collection_table.point3, point_3);
	log_printf("\n point4 %d , %d \n" ,calinfo->collection_table.point4, point_4);
	log_printf("\n point5 %d , %d \n" ,calinfo->collection_table.point5, point_5);
	log_printf("\n point6 %d , %d \n" ,calinfo->collection_table.point6, point_6);
	log_printf("\n********** END COMPARE *********\n");*/


	if(((abs(calinfo->x_axis_extr.x - x_xtr_1) < TOLERANCE) && (abs(calinfo->x_axis_extr.x - x_xtr_2) < TOLERANCE)) && ((abs(calinfo->y_axis_extr.x- y_xtr_1) < TOLERANCE) && (abs(calinfo->y_axis_extr.y- y_xtr_2) < TOLERANCE)) && ((abs(calinfo->z_axis_extr.x - z_xtr_1) < TOLERANCE) && (abs(calinfo->z_axis_extr.y - z_xtr_2) < TOLERANCE))){
	log_printf("\n MA \n");
		if(calinfo->isit6yet == isit6yet && calinfo->trip == trip && calinfo->trigger_pressed == trigger_pressed){
			log_printf("\n FA \n");
			if(calinfo->collection_table.point1 == point_1 && calinfo->collection_table.point2 == point_2 && calinfo->collection_table.point3 == point_3 && calinfo->collection_table.point4 == point_4 && calinfo->collection_table.point5 == point_5 && calinfo->collection_table.point6 == point_6){
				log_printf("\n DA \n");
				return 1;
			}
		 }
	}


	return 0;
}
bool tritoncal_calsetter_caltriggerstate(struct vector *acc_mean_vec , struct TritonCalInfo *calinfo,float averagex , float averagey , float averagez , int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6){

	acc_mean_vec->x = averagex;
	acc_mean_vec->y = averagey;
	acc_mean_vec->z = averagez;
	calinfo->x_axis_extr.x = x_xtr_1;
	calinfo->x_axis_extr.y = x_xtr_2;
	calinfo->y_axis_extr.x = y_xtr_1;
	calinfo->y_axis_extr.y = y_xtr_2;
	calinfo->z_axis_extr.x = z_xtr_1;
	calinfo->z_axis_extr.y = z_xtr_2;
	calinfo->isit6yet = isit6yet ;
	calinfo->trigger = trigger;
	calinfo->count_run = count_run;
	calinfo->trip = trip ;
	calinfo->trigger_pressed = trigger_pressed;
	calinfo->collection_table.point1 = point_1;
	calinfo->collection_table.point2 = point_2;
	calinfo->collection_table.point3 = point_3;
	calinfo->collection_table.point4 = point_4;
	calinfo->collection_table.point5 = point_5;
	calinfo->collection_table.point6 = point_6;

	return 1;
}
bool tritoncal_calchecker_caltriggerstate(struct TritonCalInfo *calinfo, int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6){
	if(((abs(calinfo->x_axis_extr.x - x_xtr_1) < TOLERANCE) && (abs(calinfo->x_axis_extr.x - x_xtr_2) < TOLERANCE)) && ((abs(calinfo->y_axis_extr.x- y_xtr_1) < TOLERANCE) && (abs(calinfo->y_axis_extr.y- y_xtr_2) < TOLERANCE)) && ((abs(calinfo->z_axis_extr.x - z_xtr_1) < TOLERANCE) && (abs(calinfo->z_axis_extr.y - z_xtr_2) < TOLERANCE))){
		 if(calinfo->isit6yet == isit6yet && calinfo->trigger == trigger && calinfo->count_run == count_run && calinfo->trip == trip && calinfo->trigger_pressed == trigger_pressed){
			if(calinfo->collection_table.point1 == point_1 && calinfo->collection_table.point2 == point_2 && calinfo->collection_table.point3 == point_3 && calinfo->collection_table.point4 == point_4 && calinfo->collection_table.point5 == point_5 && calinfo->collection_table.point6 == point_6){
				return 1;
			}
		 }
	}
	return 0;
}

/* TEST FUNCTIONS*/
bool tritoncal_sixcal_testcolorstate(){

	/*
	tritoncal_calsetter_calcolorset(struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo, float std_deviationx, float std_deviationy, float std_deviationz,  bool trigger, int count_run, int trip){
	tritoncal_calchecker_calcolorset(struct TritonCalInfo *calinfo, bool trigger, int count_run, int trip)
	*/

	struct vector acc_offset_vec;
	struct vector acc_mean_vec;
	struct vector acc_stddev_vec;
	struct TritonCalInfo calinfo;
	struct vector acc_scale_vec;
	tritoncal_sixinitialize(&acc_scale_vec,&acc_offset_vec,&acc_mean_vec, &acc_stddev_vec, &calinfo);

	bool good1 = 0 , good2 = 0 , good3 = 0 , good4 = 0, good5 = 0;
	tritoncal_calsetter_calcolorset(&acc_stddev_vec, &calinfo, 20, 15, 7, 1, 0, 0);
	tritoncal_calcolorset(&acc_stddev_vec, &calinfo);
	good1 = tritoncal_calchecker_calcolorset(&calinfo, 0, 38, 0);
	tritoncal_calsetter_calcolorset(&acc_stddev_vec, &calinfo, 9999, 9999, 9999, 1, 0, 0);
	tritoncal_calcolorset(&acc_stddev_vec, &calinfo);
	good2 = tritoncal_calchecker_calcolorset(&calinfo, 1, 0, 0);
	tritoncal_calsetter_calcolorset(&acc_stddev_vec, &calinfo, 9999, 9999, 9999, 1, 38, 0);
	tritoncal_calcolorset(&acc_stddev_vec, &calinfo);
	good3 = tritoncal_calchecker_calcolorset(&calinfo, 1, 0, 0);
	tritoncal_calsetter_calcolorset(&acc_stddev_vec, &calinfo, 20, 15, 7, 0, 600, 0);
	tritoncal_calcolorset(&acc_stddev_vec, &calinfo);
	good4 = tritoncal_calchecker_calcolorset(&calinfo, 0, 0, 1);
	tritoncal_calsetter_calcolorset(&acc_stddev_vec, &calinfo, 20, 15, 7, 0, 38, 0);
	tritoncal_calcolorset(&acc_stddev_vec, &calinfo);
	good5 = tritoncal_calchecker_calcolorset(&calinfo, 0, 76, 0);


	if(good1 && good2 && good3 && good4 && good5){
		return 1;
	}
	else{
		return 0;
	}
}
bool tritoncal_sixcal_testoffsetmath(){


	/*
	bool tritoncal_calsetter_caloffset(struct vector *acc_offset_vec, struct TritonCalInfo *calinfo , float offset_x , float offset_y, float offset_z, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2);
	bool tritoncal_calchecker_caloffset(struct TritonCalInfo *calinfo, float offset_x , float offset_y, float offset_z);
	*/

	struct vector acc_offset_vec;
	struct vector acc_mean_vec;
	struct vector acc_stddev_vec;
	struct TritonCalInfo calinfo;
	struct vector acc_scale_vec;
	tritoncal_sixinitialize(&acc_scale_vec,&acc_offset_vec,&acc_mean_vec, &acc_stddev_vec, &calinfo);

	bool good1 = 0 , good2 = 0;
	tritoncal_calsetter_caloffset(&acc_scale_vec,&acc_offset_vec, &calinfo, 9999, -9999, 9999, 8300, -8400, 8100, -8100, 8100,-8400,1,1,1);
	tritoncal_caloffset(&acc_scale_vec,&acc_offset_vec, &calinfo);
	good1 = tritoncal_calchecker_caloffset(&acc_scale_vec,&acc_offset_vec, -50, 0, -150,1,1,1);
	tritoncal_calsetter_caloffset(&acc_scale_vec,&acc_offset_vec, &calinfo, 9999, -9999, 9999, 8400, -8100, 9999, -9997, 6000, -7000,1,1,1);
	tritoncal_caloffset(&acc_scale_vec,&acc_offset_vec, &calinfo);
	good2 = tritoncal_calchecker_caloffset(&acc_scale_vec,&acc_offset_vec, 150, 1, -500,1,1,1);

	if(good1 && good2){
		return 1;
	}
	else{
		return 0;
	}
}
bool tritoncal_sixcal_testtriggerworks(){


	/*
	tritoncal_calsetter_caltriggerstate(&acc_mean_vec , &calinfo, float averagex , float averagey , float averagez , int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6);
	tritoncal_caltriggerstate(&acc_mean_vec, &calinfo);
	good1 = tritoncal_calchecker_caltriggerstate(&calinfo, int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6);
	*/

	struct vector acc_offset_vec;
	struct vector acc_mean_vec;
	struct vector acc_stddev_vec;
	struct TritonCalInfo calinfo;
	struct vector acc_scale_vec;
	tritoncal_sixinitialize(&acc_scale_vec,&acc_offset_vec,&acc_mean_vec, &acc_stddev_vec, &calinfo);

	bool good1 = 0 , good2 = 0 , good3 = 0 , good4 = 0, good5 = 0, good6 = 0, good7 = 0, good8 = 0, good9 = 0, good10 = 0;
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,8191,0,0,60,7,7,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	good1 = tritoncal_calchecker_calsteadycheck(&calinfo,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,8191,20000,0,7,7,7,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	good2 = tritoncal_calchecker_calsteadycheck(&calinfo,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,8191,0,0,7,7,7,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	good3 = tritoncal_calchecker_calsteadycheck(&calinfo,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,8191,0,0,7,7,7,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	//tritoncal_calprint_sixcal(&acc_offset_vec ,&acc_mean_vec , &acc_stddev_vec , &calinfo);
	good4 = tritoncal_calchecker_calsteadycheck(&calinfo,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,8291,0,0,7,7,7,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	//tritoncal_calprint_sixcal(&acc_offset_vec ,&acc_mean_vec , &acc_stddev_vec , &calinfo);
	good5 = tritoncal_calchecker_calsteadycheck(&calinfo,1,1,0,8291,0,0,0,0,0,1,0,0,0,0,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,-8291,0,0,7,7,7,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	//tritoncal_calprint_sixcal(&acc_offset_vec ,&acc_mean_vec , &acc_stddev_vec , &calinfo);
	good6 = tritoncal_calchecker_calsteadycheck(&calinfo,1,1,0,0,-8291,0,0,0,0,0,1,0,0,0,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,0,8291,0,7,7,7,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	//tritoncal_calprint_sixcal(&acc_offset_vec ,&acc_mean_vec , &acc_stddev_vec , &calinfo);
	good7 = tritoncal_calchecker_calsteadycheck(&calinfo,1,1,0,0,0,8291,0,0,0,0,0,1,0,0,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,0,-8291,0,7,7,7,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	//tritoncal_calprint_sixcal(&acc_offset_vec ,&acc_mean_vec , &acc_stddev_vec , &calinfo);
	good8 = tritoncal_calchecker_calsteadycheck(&calinfo,1,1,0,0,0,0,-8291,0,0,0,0,0,1,0,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,0,0,8391,7,7,7,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	//tritoncal_calprint_sixcal(&acc_offset_vec ,&acc_mean_vec , &acc_stddev_vec , &calinfo);
	good9 = tritoncal_calchecker_calsteadycheck(&calinfo,1,1,0,0,0,0,0,8391,0,0,0,0,0,1,0);
	tritoncal_calsetter_calsteadycheck(&acc_mean_vec,&acc_stddev_vec,&calinfo,0,0,-8391,7,7,7,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0);
	tritoncal_calsteadycheck(&acc_mean_vec, &acc_stddev_vec, &calinfo);
	//tritoncal_calprint_sixcal(&acc_offset_vec ,&acc_mean_vec , &acc_stddev_vec , &calinfo);
	good10 = tritoncal_calchecker_calsteadycheck(&calinfo,1,1,0,0,0,0,0,0,-8391,0,0,0,0,0,1);
	log_printf("\n %d , %d , %d , %d , %d , %d , %d , %d , %d , %d \n",good1,good2,good3,good4,good5,good6,good7,good8,good9,good10);
	if(good1 && good2 && good3 && good4 && good5 && good6 && good7 && good8 && good9 && good10){
		return 1;
	}
	else{
		return 0;
	}
}


bool tritoncal_calprint_sixcal(struct vector *acc_offset_vec ,struct vector *acc_mean_vec , struct vector *acc_stddev_vec , struct TritonCalInfo *calinfo){

	log_printf("\n acc_mean_vec->x %f \n" ,acc_mean_vec->x);
	log_printf("\n acc_mean_vec->y %f \n" ,acc_mean_vec->y);
	log_printf("\n acc_mean_vec->z %f \n" ,acc_mean_vec->z);
	log_printf("\n acc_offset_vec->x %f \n" ,acc_offset_vec->x);
	log_printf("\n acc_offset_vec->y %f \n" ,acc_offset_vec->y);
	log_printf("\n acc_offset_vec->z %f \n" ,acc_offset_vec->z);
	log_printf("\n acc_stddev_vec->x %f \n" ,acc_stddev_vec->x);
	log_printf("\n acc_stddev_vec->y %f \n" ,acc_stddev_vec->y);
	log_printf("\n acc_stddev_vec->z %f \n" ,acc_stddev_vec->z);
	log_printf("\n x_axis_extr.x %f \n" ,calinfo->x_axis_extr.x);
	log_printf("\n x_axis_extr.y %f \n" ,calinfo->x_axis_extr.y);
	log_printf("\n y_axis_extr.x %f \n" ,calinfo->y_axis_extr.x);
	log_printf("\n y_axis_extr.y %f \n" ,calinfo->y_axis_extr.y);
	log_printf("\n z_axis_extr.x %f \n" ,calinfo->z_axis_extr.x);
	log_printf("\n z_axis_extr.y %f \n" ,calinfo->z_axis_extr.y);
	log_printf("\n isit6yet %d \n" ,calinfo->isit6yet);
	log_printf("\n trigger %d \n" ,calinfo->trigger);
	log_printf("\n count_run %d \n" ,calinfo->count_run);
	log_printf("\n trip %d \n" ,calinfo->trip);
	log_printf("\n trigger_pressed %d \n" ,calinfo->trigger_pressed);
	log_printf("\n point1 %d \n" ,calinfo->collection_table.point1);
	log_printf("\n point2 %d \n" ,calinfo->collection_table.point2);
	log_printf("\n point3 %d \n" ,calinfo->collection_table.point3);
	log_printf("\n point4 %d \n" ,calinfo->collection_table.point4);
	log_printf("\n point5 %d \n" ,calinfo->collection_table.point5);
	log_printf("\n point6 %d \n" ,calinfo->collection_table.point6);

	return 1;
}






