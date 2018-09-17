#pragma once

#include "TritonDefs.h"
// remember to uncomment line 809 int tritondefs

void tritoncal_cal_newwin(struct TritonCalInfo *calinfo,int Nwin);
void tritoncal_calGyro(struct TritonCalInfo *calinfo, struct TritonFeatures *New);
void tritoncal_calAcc(struct TritonCalInfo *calinfo, struct TritonFeatures *New);

/* KAP CALI FUNCTIONS and SUB FUNCTIONS*/
int tritoncal_sixcal(struct vector *acc_scale_vec,struct vector *acc_offset_vec,struct vector *acc_mean_vec, struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo);
int tritoncal_calsteadycheck(struct vector *acc_mean_vec, struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo);
int tritoncal_caloffset(struct vector *acc_scale_vec,struct vector *acc_offset_vec, struct TritonCalInfo *calinfo);
bool tritoncal_calcolorset(struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo);
int tritoncal_caltriggerstate(struct vector *acc_mean_vec, struct TritonCalInfo *calinfo);


/* valid input checker */
bool tritoncal_sixcalinputvalid(struct vector *acc_offset_vec,struct vector *acc_mean_vec, struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo);


/* TEST RESULTS */
void tritoncal_caltestresults();

/* INITIALIZORS */
bool tritoncal_sixinitialize(struct vector *acc_scale_vec,struct vector *acc_offset_vec,struct vector *acc_mean_vec, struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo);

/* SETTERS AND CHECKERS FUNCTION */
bool tritoncal_calsetter_sixcal(struct vector *acc_offset_vec ,struct vector *acc_mean_vec , struct vector *acc_stddev_vec , struct TritonCalInfo *calinfo ,float averagex , float averagey , float averagez , float std_deviationx, float std_deviationy, float std_deviationz, float offset_x , float offset_y, float offset_z, int isit6yet, bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5, bool point_6);
bool tritoncal_calchecker_sixcal(struct vector *acc_offset_vec ,struct vector *acc_mean_vec , struct vector *acc_stddev_vec , struct TritonCalInfo *calinfo, float offset_x , float offset_y, float offset_z, int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6);
bool tritoncal_calsetter_caloffset(struct vector *acc_scale_vec,struct vector *acc_offset_vec, struct TritonCalInfo *calinfo , float offset_x , float offset_y, float offset_z, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, float scale_x , float scale_y, float scale_z);
bool tritoncal_calchecker_caloffset(struct vector *acc_scale_vec,struct vector *acc_offset_vec, float offset_x , float offset_y, float offset_z, float scale_x , float scale_y, float scale_z);
bool tritoncal_calsetter_calcolorset(struct vector *acc_stddev_vec, struct TritonCalInfo *calinfo, float std_deviationx, float std_deviationy, float std_deviationz,  bool trigger, int count_run, int trip);
bool tritoncal_calchecker_calcolorset(struct TritonCalInfo *calinfo, bool trigger, int count_run, int trip);
bool tritoncal_calsetter_caltriggerstate(struct vector *acc_mean_vec , struct TritonCalInfo *calinfo,float averagex , float averagey , float averagez , int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6);
bool tritoncal_calchecker_caltriggerstate(struct TritonCalInfo *calinfo, int isit6yet , bool trigger, int count_run, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6);
bool tritoncal_calsetter_calsteadycheck(struct vector *acc_mean_vec , struct vector *acc_stddev_vec , struct TritonCalInfo *calinfo,float averagex , float averagey , float averagez , float std_deviationx, float std_deviationy, float std_deviationz, int isit6yet, int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6);
bool tritoncal_calchecker_calsteadycheck(struct TritonCalInfo *calinfo, int isit6yet , int trip, bool trigger_pressed, float x_xtr_1, float x_xtr_2,  float y_xtr_1, float y_xtr_2, float z_xtr_1, float z_xtr_2, bool point_1 , bool point_2 , bool point_3 , bool point_4 , bool point_5 , bool point_6);

/* TEST FUNCTIONS */
bool tritoncal_sixcal_testcolorstate();
bool tritoncal_sixcal_testoffsetmath();
bool tritoncal_sixcal_testtriggerworks();

/*PRINT FUNCTION*/
bool tritoncal_calprint_sixcal(struct vector *acc_offset_vec ,struct vector *acc_mean_vec , struct vector *acc_stddev_vec , struct TritonCalInfo *calinfo);

/*
struct pointcollect
{
	bool point1;
	bool point2;
	bool point3;
	bool point4;
	bool point5;
	bool point6;

};
*/
/*
struct TritonCalInfo
{
	struct vector gyro_offset;
	struct vector acc_offset;
	struct float2_vector x_axis_extr;
	struct float2_vector y_axis_extr;
	struct float2_vector z_axis_extr;
	struct pointcollect collection_table;
	int isit6yet;
    bool trigger;
    int count_run;
    int trip;
    bool trigger_pressed;

};
*/
