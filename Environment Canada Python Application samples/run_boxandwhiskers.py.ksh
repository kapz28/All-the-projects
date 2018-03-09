#! /usr/bin/ksh
# run_timeconvert.py.ksh

# This script launches timeconvert.py

# Written by Kapilan Satkunanathan, October 2017

###################################################################################################33
datestr_str=2016060100
#dateend_str=2016063012
dateend_str=2016083112
station_list=~/diskspace/timeseries/FINAL_STATION_LIST_filtered_by_available_shapefile.csv
dir_in_master_files=~/diskspace/test_milena/data/v3/
obs_hydat_data=~/diskspace/test_dumpv3/obs_hydat_anonmaly_average.csv
dir_out=~/diskspace/test_dumpv4/
dir_outv2=~/diskspace/test_dumpv5/
set -x 
python boxandwhiskers.py  $datestr_str  $dateend_str  $station_list  $dir_in_master_files $obs_hydat_data $dir_out $dir_outv2
set +x


