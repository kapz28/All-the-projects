
######################################################################
import csv                                                           # Import modules
import datetime
import os
import string
import sys
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import matplotlib.ticker as mticker
import math
import random

class Timeseries:
        def boxandwhiskers(self,datestr_str,dateend_str, dir_out):

            datestart = datetime.datetime.strptime(datestr_str, "%Y%m%d%H")# start date
            datecurr = datetime.datetime.strptime(datestr_str, "%Y%m%d%H") # current date
            dateend = datetime.datetime.strptime(dateend_str, "%Y%m%d%H")  # end date
            numdates = (((dateend - datestart).days) + 1)                # The number of dates to be processed: Nsyntimes synoptic times per day, including on date_start and date_end
	    numberofcorr = 19
	    lead_time=True
	    x_axis = []
	    y = []
	    y = np.array(y)
	    '''
	    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y%m%d'))
            plt.gca().xaxis.set_major_locator(mdates.DayLocator())
	    fig = plt.figure(1, figsize=(9,6))
	    mpl.use('agg')
	    ax = fig.add_subplot(111)
	    '''
	    i = 0 
	    for dates_x in range(0,numdates):

		file_in_name = "corr_%04d%02d%02d.csv"%(datecurr.year , datecurr.month , datecurr.day)
		file_in_path = os.path.join(dir_out,file_in_name)
		avercorr_open = open(file_in_path, 'rb')
		avercorr_read = csv.reader(avercorr_open)
		avercorr_list = list(avercorr_read)
		avercorr_array_1 = np.array(avercorr_list[1])

		if i == 0:
			temp = np.array(avercorr_array_1)
		else: 
			temp = np.vstack([temp,avercorr_array_1])
		i = i+1
		datecurr += datetime.timedelta(days=1)               #increment date by one 

	    datecurr = datestart

	    for dates_x in range(0,numdates):
		temp_date ="%04d%02d%02d"%(datecurr.year , datecurr.month , datecurr.day)
		x_axis.append(temp_date)
		datecurr += datetime.timedelta(days=1)               #increment date by one 

	    temp = np.array(temp)
	    y = temp.astype(np.float)
	    if lead_time == True:
	    	y = np.transpose(y)
	    
	    x = np.array(x_axis)
	    #################custom x -list(hard code)
	    x = []

	    for upby in range(24,240+1,12):
	    	x.append(str(upby))
	    #################custom x -list(hard code)

	    print "\n y is", y 	 
	    print "\n x is", x
	    print "\n len y is", len(y)
	    print "\n len y is", len(y[1])
	    print "\n len x is", len(x)
	    datecurr = datestart
	    mpl.use('agg')
	    fig = plt.figure(1, figsize=(20,13))
	    ax = fig.add_subplot(111)
	    bp = plt.boxplot(y)
	    #bp = plt.boxplot(y, x, widths=(0.1))
	    fig.savefig('fig1.png', bbox_inches='tight')
	    for tick in ax.xaxis.get_major_ticks():
	    	tick.label.set_fontsize(10)
	    ax.set_xticklabels(x, rotation=55)
	    #myLocator = mticker.MultipleLocator(4)
	    #ax.xaxis.set_major_locator(myLocator)
	    plt.rc('xtick', labelsize=10)    # fontsize of the tick labels
	    plt.rc('axes', labelsize=1)    # fontsize of the x and y labels
	    plt.show()
	    '''
	    ax.set_xticklabels(x_axis)
	    y = []
	    empty_list = [None]* len(x_axis)
	    list_count = 0
	    datecurr = datestart
	    
	    for dates_x in range(0,numdates):

		file_in_name = "corr_%04d%02d%02d.csv"%(datecurr.year , datecurr.month , datecurr.day)
		file_in_path = os.path.join(dir_out,file_in_name)
		avercorr_open = open(file_in_path, 'rb')
		avercorr_read = csv.reader(avercorr_open)
		avercorr_list = list(avercorr_read)
		y.append(avercorr_list[1])

	    print "\n y is", y
	    print "\n len y is", len(y)
	    y = numpy.array(y)
	    bp = ax.boxplot(y)
	    fig.savefig('test_box.png', bbox_inches='tight')
	    
	    plt.rc('xtick', labelsize=10)    # fontsize of the tick labels
	    plt.rc('axes', labelsize=4)    # fontsize of the x and y labels
	    plt.rc('font', labelsize=4)    # fontsize of the x and y labels
	    plt.gcf().autofmt_xdate() 
	    plt.show()
	    '''


if __name__ == '__main__':
    if len(sys.argv) < 8:
        msg = "".join(("You need at least 7 arguments here and provided: {}\n\n",
                       "Correct usage is : {} datestr_str dateend_str station_list timeconversion_list dir_in_hydat dir_out dir_offset_files\n\n",
                       "        datestr_str : start date for the verification yyyymmdd\n",
                       "        dateend_str : end date for the verification yyyymmdd\n",
                       "        station_list : contains all the stations code to be processedi\n",
                       "        dir_in_master_files: contains directory of all shapefilesand anonmaly intersection\n",
                       "        obs_hydat_data: hydat and observation anonmaly data\n",
                       "        dir_out : path of directory to receive the generated image\n"))
        raise TypeError(msg.format(len(sys.argv)-1, sys.argv[0]))
    Timeseries().boxandwhiskers(sys.argv[1], sys.argv[2], sys.argv[6])
