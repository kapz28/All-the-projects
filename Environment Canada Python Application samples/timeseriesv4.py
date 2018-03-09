
######################################################################
import csv                                                           # Import modules
import datetime
import os
import string
import sys
import numpy
import shutil
import rpnpy.librmn.all as rmn                                       # For Python RPN v2.0
from pylab import *
from matplotlib import gridspec
import scipy.stats.stats
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import rpnstd
import subprocess, string
import glob
import time
import math
import random
import matplotlib.ticker as ticker

class Timeseries:
        def calccorrelation(self,datestr_str,dateend_str, station_list, dir_in_master_files, obs_hydat_data, dir_out):
            print "dai"

            #Initialization of varibles
            datestart = datetime.datetime.strptime(datestr_str, "%Y%m%d%H")# start date
            datecurr = datetime.datetime.strptime(datestr_str, "%Y%m%d%H") # current date
            dateend = datetime.datetime.strptime(dateend_str, "%Y%m%d%H")  # end date
            numdates = (((dateend - datestart).days) + 1)                # The number of dates to be processed: Nsyntimes synoptic times per day, including on date_start and date_end
	    obs_day_shift_index = 1
	    forecast_hour_day_range = 9 				# if 240 hour max forecast hour then 10 days 
	    forecast_hour_day_range_max = 240
	    forecast_hour_day_range_max_reset = 240 
	    forecast_hour_step = 12
	    amount_of_correlations=19
	    missing_val = -999.999
	    missing_val_obs = -9999.9999
	    anon_threshold = float(-900)
	    upper_anon_threshold = float(900)
	    GEPS_threshold = float(2)
	    upper_GEPS_threshold = float(3)
            # Checking if start date is not greater than end date
            if dateend < datestart:
                 print 'ERROR: the end date', dateend.strftime("%Y%m%d%H"), 'is earlier than the start date', datestart.strftime("%Y%m%d%H")
                 print 'Aborting'
                 quit()

	    station_list_opened = open(station_list, 'rb')
	    station_list_read = csv.reader(station_list_opened)
	    station_list_list = list(station_list_read)               #convert the csv file to list so it is much easier to handle
            list_station_transpose=zip(*station_list_list)    #transpose the array
            list_station = list_station_transpose[0]
            station_array= list(list_station)
            station_array.pop(0)
            print station_array


	    '''
	    the following line of code is for testing purposes
            '''
#	    station_array = ["01AL002","01AN002","01BC001"]
	    '''
	    the following line of code is for testing purposes
            '''
	    temp_write_list = []	    


	    obs_hydat_data_open = open(obs_hydat_data, 'rb')
	    obs_hydat_data_read = csv.reader(obs_hydat_data_open)
	    obs_hydat_data_list_turn = list(obs_hydat_data_read)               #convert the csv file to list so it is much easier to handle
	    obs_hydat_data_list=zip(*obs_hydat_data_list_turn)    #transpose the array
	    obs_hydat_data_list = list(obs_hydat_data_list)
	    obs_hydat_data_array = numpy.array(obs_hydat_data_list)
	    obs_hydat_data_array = numpy.delete(obs_hydat_data_array,0, 0)

	    for index_obs_day in range(0,numdates):

		file_out_name = "corr_%04d%02d%02d.csv"%(datecurr.year , datecurr.month , datecurr.day)
		file_out_path = os.path.join(dir_out,file_out_name)
		file_out_open = open(file_out_path, 'wb')
		file_out_write = csv.writer(file_out_open, dialect='excel')
#		temp_write_list.append(datecurr)

		for header in range(1,amount_of_correlations+1):

			headerstr= "corr_%d"%(header)
			temp_write_list.append(headerstr)

		file_out_write.writerow(temp_write_list)
		temp_write_list= []
		print "\n obs_hydat_corr_input", obs_hydat_data_array
		print "\n index_obs_day",index_obs_day
		print "\n length of obs_hydat", len(obs_hydat_data_array)
		print "\n numdates", numdates
		obs_hydat_corr_input = obs_hydat_data_array[index_obs_day]
		datemaster_1 = datecurr
		datemaster_2 = datecurr
		datemaster_2 -= datetime.timedelta(days=forecast_hour_day_range)               #increment date by one 
		equalized = False

		while datemaster_2 <= datemaster_1:

			 file_in_name = "master_%04d%02d%02d%02d.csv"%(datemaster_2.year , datemaster_2.month , datemaster_2.day  ,datemaster_2.hour)
			 file_in_path = os.path.join(dir_in_master_files,file_in_name)

			 if (os.path.isfile(file_in_path)  ==  True):
				
				master_open = open(file_in_path, 'rb')   
				master_read = csv.reader(master_open)
				master_list_turn = list(master_read)
				master_list_tuple=zip(*master_list_turn)    #transpose the array
				master_list = list(master_list_tuple)
				master_array = numpy.array(master_list)
				master_array = numpy.delete(master_array,0, 0)
				master_array = numpy.delete(master_array,0, 1)
				y = int(math.trunc(forecast_hour_day_range_max/24.0))	
				print "\n master_array",master_array
				print '\n forecast_hour_day_range_max', forecast_hour_day_range_max
				print '\n file_in_path', file_in_path
				print '\n file_out_path', file_out_path
				print '\n y ', y
				a = obs_hydat_corr_input 
				numrows = len(master_array)    # 3 rows in your example
				numcols = len(master_array[0])
				print "\n numrows", numrows
				print "\n numcols", numcols
				b = master_array[y-1]
				a = list(map(float,a))
				b = list(map(float,b))
				a = numpy.array(a)
				b = numpy.array(b)
				print "\n a1",a
				print "\n b1",b

				machan = numpy.where(a == float(missing_val))
				print "\n machan", machan
                                x_coordinate = machan[0]
                                print "\n x coordinate a", x_coordinate
	
                                for index in range(0, len(x_coordinate)):
                                        x = x_coordinate[index]
					if not  x > (len(b)-1):
						b[x] = (missing_val*2.0)

				machan = numpy.where(b == missing_val)
                                x_coordinate = machan[0]
                                print "\n x coordinate b", x_coordinate

                                for index in range(0, len(x_coordinate)):
                                        x = x_coordinate[index]
					if not  x > (len(a)-1):
						a[x] = (missing_val*2.0)

				machan = numpy.where(b > upper_anon_threshold)
                                x_coordinate = machan[0]
                                print "\n x coordinate b", x_coordinate

                                for index in range(0, len(x_coordinate)):
                                        x = x_coordinate[index]
					if not  x > (len(a)-1):
						a[x] = float(missing_val_obs)
						b[x] = float(missing_val_obs)
				
				machan = numpy.where(b < anon_threshold)
                                x_coordinate = machan[0]
                                print "\n x coordinate b", x_coordinate

                                for index in range(0, len(x_coordinate)):
                                        x = x_coordinate[index]
					if not  x > (len(a)-1):
						a[x] = float(missing_val_obs)
						b[x] = float(missing_val_obs)
				print "\n a2",a
				print "\n b2",b

				machan = numpy.where(a > upper_GEPS_threshold)
                                x_coordinate = machan[0]
                                print "\n x coordinate b", x_coordinate

                                for index in range(0, len(x_coordinate)):
                                        x = x_coordinate[index]
					if not  x > (len(a)-1):
						a[x] = float(missing_val_obs)
						b[x] = float(missing_val_obs)

				machan = numpy.where(a < GEPS_threshold)
                                x_coordinate = machan[0]
                                print "\n x coordinate b", x_coordinate

                                for index in range(0, len(x_coordinate)):
                                        x = x_coordinate[index]
					if not  x > (len(a)-1):
						a[x] = float(missing_val_obs)
						b[x] = float(missing_val_obs)

				a = list(a)
				b = list(b)
				a = filter(lambda d: d != missing_val_obs, a)	
				b = filter(lambda d: d != missing_val_obs, b)	
				a = filter(lambda d: d != missing_val, a)	
				b = filter(lambda e: e != missing_val, b)
				print "\n a3",a
				print "\n b3",b
				a = filter(lambda f: f != (missing_val*2.0), a)	
				b = filter(lambda g: g != (missing_val*2.0), b)
				print "\n a4",a
				print "\n b4",b
				if len(b) == 0  or len(a) == 0:	# if either array is empty
					corr = numpy.nan
					print "\nnan"
				else:
					a = numpy.array(a)
					b = numpy.array(b)
					corr_list = scipy.stats.stats.spearmanr(a,b)	#####$$$$$$$$$$$$$$$$$$$$$note to self fix corr_list outputs a list of correlation values only want one value
					print "\n corr_list", corr_list
					print "\n datemaster_2",datemaster_2
					print "\n forecast_hour_day_range_max", forecast_hour_day_range_max
					print "\n file_in_path", file_in_path
					corr = float(corr_list[0])
				temp_write_list.append(corr)
				master_open.close()	
			 forecast_hour_day_range_max -= forecast_hour_step

			 datemaster_2 += datetime.timedelta(hours=forecast_hour_step)	

		while len(temp_write_list) < 19:
			temp_write_list.insert(0,numpy.nan)
		print "\n writerow", temp_write_list
		file_out_write.writerow(temp_write_list)
		temp_write_list = []
		forecast_hour_day_range_max = forecast_hour_day_range_max_reset
				
	        file_out_open.close()
	        datecurr += datetime.timedelta(days=1)               #increment date by one 

	
        def anoncorrelation(self,datestr_str,dateend_str, station_list, dir_in_master_files, obs_hydat_data, dir_out):
            print "dai"

            #Initialization of varibles
            datestart = datetime.datetime.strptime(datestr_str, "%Y%m%d%H")# start date
            datecurr = datetime.datetime.strptime(datestr_str, "%Y%m%d%H") # current date
            dateend = datetime.datetime.strptime(dateend_str, "%Y%m%d%H")  # end date
            numdates = (((dateend - datestart).days) + 1)                # The number of dates to be processed: Nsyntimes synoptic times per day, including on date_start and date_end
	    obs_day_shift_index = 1
	    forecast_hour_day_range = 9 				# if 240 hour max forecast hour then 10 days 
	    forecast_hour_day_range_max = 240
	    forecast_hour_day_range_max_reset = 240 
	    forecast_hour_step = 12
	    amount_of_correlations=19
	    missing_val = -999.999
	    numberofcorr = 19
	    anon_threshold = float(30)
	    x_axis = []
	    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y%m%d'))
            plt.gca().xaxis.set_major_locator(mdates.DayLocator())

	    for dates_x in range(0,numdates):
		x_axis.append(datecurr)
		datecurr += datetime.timedelta(days=1)               #increment date by one 
            # Checking if start date is not greater than end date
            if dateend < datestart:
                 print 'ERROR: the end date', dateend.strftime("%Y%m%d%H"), 'is earlier than the start date', datestart.strftime("%Y%m%d%H")
                 print 'Aborting'
                 quit()


	    obs_hydat_data_open = open(obs_hydat_data, 'rb')
	    obs_hydat_data_read = csv.reader(obs_hydat_data_open)
	    obs_hydat_data_list = list(obs_hydat_data_read)               #convert the csv file to list so it is much easier to handle
	    obs_hydat_data_array = numpy.array(obs_hydat_data_list)
	    obs_hydat_data_array = numpy.delete(obs_hydat_data_array,0, 1)
	    obs_hydat_data_array = obs_hydat_data_array.astype(np.float)
	    print "obs 1",obs_hydat_data_array
	    obs_hydat_data_list = list(obs_hydat_data_array)
	    print "obs 2",obs_hydat_data_list
	    for i in range(len(obs_hydat_data_list)):
			for j in range(len(obs_hydat_data_list[i])):
				if obs_hydat_data_list[i][j] == missing_val:#  or obs_hydat_data_list[i][j] > float(anon_threshold):                ***********************filter is here****************
					obs_hydat_data_list[i][j] = None
	    sum_hold = 0
	    average_list_sum = []
	    obs_hydat_data_array = numpy.array(obs_hydat_data_list)
	    print "obs 3",obs_hydat_data_array
	    for i in range(len(obs_hydat_data_array[0])):
			sum_hold = 0
			items = len(obs_hydat_data_array)
			for j in range(len(obs_hydat_data_array)):
				if numpy.isnan(obs_hydat_data_array[j][i]) == False:
					print "val", obs_hydat_data_array[j][i]
					sum_hold += float(obs_hydat_data_array[j][i])
				else:
					items = items - 1
			if items != 0:
				print "\n sum hold", sum_hold
				print "\n items", items
				average_list_sum.append(float(sum_hold/items))
			else:
				print "pundai"
				average_list_sum.append(None)
	    print "length of average_list_sum", len(average_list_sum)
	    print "length of x_axis", len(x_axis)
	    print " average_list_sum", average_list_sum
	    print "x_axis", x_axis
	    plt.plot(x_axis,average_list_sum,linestyle='-', marker='o')
	    plt.gcf().autofmt_xdate() 
	    plt.show()

        def daycorrelation(self,datestr_str,dateend_str, station_list, dir_in_master_files, obs_hydat_data, dir_outv2):

            #Initialization of varibles
            datestart = datetime.datetime.strptime(datestr_str, "%Y%m%d%H")# start date
            datecurr = datetime.datetime.strptime(datestr_str, "%Y%m%d%H") # current date
            dateend = datetime.datetime.strptime(dateend_str, "%Y%m%d%H")  # end date
            numdates = (((dateend - datestart).days) + 1)                # The number of dates to be processed: Nsyntimes synoptic times per day, including on date_start and date_end
	    obs_day_shift_index = 1
	    forecast_hour_day_range = 9 				# if 240 hour max forecast hour then 10 days 
	    forecast_hour_day_range_span = 9 				# if 240 hour max forecast hour then 10 days 
	    forecast_hour_day_range_max = 240
	    forecast_hour_day_range_max_reset = 240 
	    forecast_hour_step = 12
	    amount_of_correlations=19
	    missing_val = -999.999
	    anon_threshold = float(-900)
	    GEPS_threshold = float(2)
	    upper_GEPS_threshold= float(3)
	    upper_anon_threshold=float(900)
	    missing_val_obs = -9999.9999
            # Checking if start date is not greater than end date
            if dateend < datestart:
                 print 'ERROR: the end date', dateend.strftime("%Y%m%d%H"), 'is earlier than the start date', datestart.strftime("%Y%m%d%H")
                 print 'Aborting'
                 quit()

	    station_list_opened = open(station_list, 'rb')
	    station_list_read = csv.reader(station_list_opened)
	    station_list_list = list(station_list_read)               #convert the csv file to list so it is much easier to handle
            list_station_transpose=zip(*station_list_list)    #transpose the array
            list_station = list_station_transpose[0]
            station_array= list(list_station)
            station_array.pop(0)
            print "station array" ,station_array


	    '''
	    the following line of code is for testing purposes
            '''
#	    station_array = ["01AL002","01AN002","01BC001"]
	    '''
	    the following line of code is for testing purposes
            '''
	    temp_write_list = []	    
	    temp_obs_corr = None

	    obs_hydat_data_open = open(obs_hydat_data, 'rb')
	    obs_hydat_data_read = csv.reader(obs_hydat_data_open)
	    obs_hydat_data_list = list(obs_hydat_data_read)               #convert the csv file to list so it is much easier to handle
	    obs_hydat_data_array = numpy.array(obs_hydat_data_list)
	    print "obs_hydat_data_array", obs_hydat_data_list[0]
	    dateend_reset = dateend

	    empty_list = [None]* int(2* numdates)
	
	    	
	    for day in range(0,forecast_hour_day_range_span):
		file_out_name = "daycorr_anon_test_filtered_%d.csv"%(day)
		file_out_path = os.path.join(dir_outv2,file_out_name)
		file_out_open = open(file_out_path, 'wb')
		file_out_write = csv.writer(file_out_open, dialect='excel')
		dateend = dateend_reset
		datecurr = datestart
		if day != 0:
			for shift in range(0, day):
				datecurr -= datetime.timedelta(days=1) 
				dateend -= datetime.timedelta(days=1)  
		datecurr_reset = datecurr
		             
            	for station in station_array:
			datecurr = datecurr_reset
			coordinates = numpy.where(obs_hydat_data_array == str(station))
			print "\n coordinates", coordinates
			print "\n station", station
			coordinates_x = coordinates[0][0]
			temp_obs_corr = obs_hydat_data_list[coordinates_x]
			print "\n temp_obs_corr before", temp_obs_corr
			temp_obs_corr.pop(0)
			print "\n temp_obs_corr after",temp_obs_corr

			for index in range(0,(len(temp_obs_corr)*2),2):
				empty_list[index] = temp_obs_corr[int(index/2.0)]
				empty_list[index+1] = temp_obs_corr[int(index/2.0)]
			print "\n empty list", empty_list
	    		empty_list_GEPS = [None]* int(2* numdates)
			i = 0

			while datecurr <= dateend:	
				datetemp = datecurr
				file_in_name = "master_%04d%02d%02d%02d.csv"%(datetemp.year , datetemp.month , datetemp.day  ,datetemp.hour)
				file_in_path = os.path.join(dir_in_master_files,file_in_name)
				print "\nfile_in_path", file_in_path

				if (os.path.isfile(file_in_path)  ==  True):
					master_open = open(file_in_path, 'rb')   
					master_read = csv.reader(master_open)
					master_list = list(master_read)
					master_array = numpy.array(master_list)
					master_array = numpy.delete(master_array,0, 0)
					station_search = "%s_1"%(station)
					coordinates = numpy.where(master_array == str(station_search))
					print "\n master_array", master_array
					print "\n coordinates", coordinates
					print "\n station", station
					print "\nfile_in_path", file_in_path
					print "\n datecurr",datecurr
					print "\n datetemp",datetemp
					print "\n dateend", dateend
					print "\n i", i
					print "\n day", day
					coordinates_x = coordinates[0][0]
					temp_GEPS= list(master_array[coordinates_x])
					temp_GEPS.pop(0)
					print "\n value",temp_GEPS[day]
					empty_list_GEPS[i]= temp_GEPS[day]
	        		datetemp -= datetime.timedelta(hours=12)                
				file_in_name = "master_%04d%02d%02d%02d.csv"%(datetemp.year , datetemp.month , datetemp.day  ,datetemp.hour)
				file_in_path = os.path.join(dir_in_master_files,file_in_name)

				if (os.path.isfile(file_in_path)  ==  True):
					master_open = open(file_in_path, 'rb')   
					master_read = csv.reader(master_open)
					master_list = list(master_read)
					master_array = numpy.array(master_list)
					master_array = numpy.delete(master_array,0, 0)
					station_search = "%s_1"%(station)
					coordinates = numpy.where(master_array == str(station_search))
					print "\n master_array", master_array
					print "\n coordinates", coordinates
					print "\n station", station
					print "\n file_in_path", file_in_path			
					print "\n datecurr",datecurr
					print "\n datetemp",datetemp
					print "\n dateend", dateend
					print "\n i", i
					print "\n day", day
					coordinates_x = coordinates[0][0]
					temp_GEPS= list(master_array[coordinates_x])
					temp_GEPS.pop(0)
					empty_list_GEPS[i+1]= temp_GEPS[day]
					print "\n value",temp_GEPS[day]
				i = i + 2
	        		datecurr += datetime.timedelta(days=1)               #increment date by one
			a = list(empty_list_GEPS)
			b = list(empty_list)
			print "\nlen a", len(a)
			print "\nlen b", len(b)
			print "\n day", day
			print "\n a1",a
			print "\n b1",b
			print "\n datecurr",datecurr
			print "\n dateend", dateend
			print "\n i", i
			a = [float(i) for i in a]
			b = [float(j) for j in b]
			print "\n a2",a
			print "\n b2",b
#######################
			a = numpy.array(a)
			b = numpy.array(b)
#######################
			print "\n a1",a
			print "\n b1",b

			machan = numpy.where(a == float(missing_val))
			print "\n machan", machan
			x_coordinate = machan[0]
			print "\n x coordinate a", x_coordinate

			for index in range(0, len(x_coordinate)):
				x = x_coordinate[index]
				if not  x > (len(b)-1):
					b[x] = (missing_val*2.0)

			machan = numpy.where(b == missing_val)
			x_coordinate = machan[0]
			print "\n x coordinate b", x_coordinate

			for index in range(0, len(x_coordinate)):
				x = x_coordinate[index]
				if not  x > (len(a)-1):
					a[x] = (missing_val*2.0)

			machan = numpy.where(b > upper_anon_threshold)
			x_coordinate = machan[0]
			print "\n x coordinate b", x_coordinate

			for index in range(0, len(x_coordinate)):
				x = x_coordinate[index]
				if not  x > (len(a)-1):
					a[x] = float(missing_val_obs)
					b[x] = float(missing_val_obs)
			
			machan = numpy.where(b < anon_threshold)
			x_coordinate = machan[0]
			print "\n x coordinate b", x_coordinate

			for index in range(0, len(x_coordinate)):
				x = x_coordinate[index]
				if not  x > (len(a)-1):
					a[x] = float(missing_val_obs)
					b[x] = float(missing_val_obs)
			print "\n a2",a
			print "\n b2",b

			machan = numpy.where(a > upper_GEPS_threshold)
			x_coordinate = machan[0]
			print "\n x coordinate b", x_coordinate

			for index in range(0, len(x_coordinate)):
				x = x_coordinate[index]
				if not  x > (len(a)-1):
					a[x] = float(missing_val_obs)
					b[x] = float(missing_val_obs)

			machan = numpy.where(a < GEPS_threshold)
			x_coordinate = machan[0]
			print "\n x coordinate b", x_coordinate

			for index in range(0, len(x_coordinate)):
				x = x_coordinate[index]
				if not  x > (len(a)-1):
					a[x] = float(missing_val_obs)
					b[x] = float(missing_val_obs)

			a = list(a)
			b = list(b)
#######################
			'''
			a = self.replacev2(b,a,GEPS_threshold,missing_val)
			b = self.replacev2(a,b,anon_threshold,missing_val)
			b = self.replace(a,b,missing_val,(missing_val*2.0))
			a = self.replace(b,a,missing_val,(missing_val*2.0))
			'''
			a = filter(lambda d: d != missing_val, a)	
			b = filter(lambda e: e != missing_val, b)
			print "\n a3",a
			print "\n b3",b
			a = filter(lambda f: f != (missing_val*2.0), a)	
			b = filter(lambda g: g != (missing_val*2.0), b)
			a = filter(lambda f: f != (missing_val_obs), a)	
			b = filter(lambda g: g != (missing_val_obs), b)
			print "\n a4",a
			print "\n b4",b
			if len(b) == 0  or len(a) == 0:	# if either array is empty
				corr = numpy.nan
				print "\nnan"
			else:
				print "\n a5",a
				print "\n b5",b
				a = numpy.array(a)
				b = numpy.array(b)
				print "\nlen a", len(a)
				print "\nlen b", len(b)
				a = a.astype(np.float)
				b = b.astype(np.float)
				print "\n a6",a
				print "\n b6",b
				corr_list = scipy.stats.stats.spearmanr(a,b)	#####$$$$$$$$$$$$$$$$$$$$$note to self fix corr_list outputs a list of correlation values only want one value
				print "\n corr_list", corr_list
				corr = float(corr_list[0])
			temp_write_list.append(station)
			temp_write_list.append(corr)
			file_out_write.writerow(temp_write_list)
	    		temp_write_list = []	    
		file_out_open.close()

        def plotcorrelation(self,datestr_str,dateend_str, dir_out):
            datestart = datetime.datetime.strptime(datestr_str, "%Y%m%d%H")# start date
            datecurr = datetime.datetime.strptime(datestr_str, "%Y%m%d%H") # current date
            dateend = datetime.datetime.strptime(dateend_str, "%Y%m%d%H")  # end date
            numdates = (((dateend - datestart).days) + 1)                # The number of dates to be processed: Nsyntimes synoptic times per day, including on date_start and date_end
	    numberofcorr = 19
	    x_axis = []
	    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y%m%d'))
            plt.gca().xaxis.set_major_locator(mdates.DayLocator())

	    for dates_x in range(0,numdates):
		x_axis.append(datecurr)
		datecurr += datetime.timedelta(days=1)               #increment date by one 

	    y = []
	    empty_list = [None]* len(x_axis)
	    list_count = 0
	    datecurr = datestart
	    for day_range in range (0,numberofcorr): # filter out per forecast hour by changing start of for loop
#	    for day_range in range (0,numberofcorr):    # original 
	            day_range_offset =day_range + 1
	            true_day_range = math.ceil(day_range_offset/2.0)

		    for dates_x in range(0,numdates):
			file_in_name = "corr_%04d%02d%02d.csv"%(datecurr.year , datecurr.month , datecurr.day)
			file_in_path = os.path.join(dir_out,file_in_name)
			avercorr_open = open(file_in_path, 'rb')
			avercorr_read = csv.reader(avercorr_open)
			avercorr_list = list(avercorr_read)
			y.append(avercorr_list[1][day_range])
			print "\navercorr_list", avercorr_list[1][day_range]
			if numpy.isnan(float(avercorr_list[1][day_range])) == True:
				print "\nyay"
				empty_list[dates_x]= None	
			else:
				empty_list[dates_x] = float(avercorr_list[1][day_range])
			if len(y) == true_day_range or (numdates-1) == dates_x:
				y = []
				print "\nempty list is", empty_list		
				print "\nx axis", x_axis		
				if any(empty_list):
					y_axis = np.array(empty_list).astype(np.double)
					print "\ny axis", y_axis		
					ymask = np.isfinite(y_axis)
					ymask[:] = [not x for x in ymask]
					numpy.array(x_axis)
					numpy.array(empty_list)
					print "sizes ", len(empty_list),len(x_axis), len(ymask) 
					#ymask = list(ymask)
					print "\nymask", ymask
	#################################plot after###########
	#				plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%Y%m%d'))
	#				plt.gca().xaxis.set_major_locator(mdates.DayLocator())
					plt.rc('xtick', labelsize=10)    # fontsize of the tick labels
					plt.rc('axes', labelsize=1)    # fontsize of the x and y labels
					plt.plot(x_axis,empty_list,linestyle='-', marker='o')
#					plt.plot(x_axis[ymask],empty_list[ymask],linestyle='-', marker='o')
	    			empty_list = [None]* len(x_axis)

			datecurr += datetime.timedelta(days=1)               #increment date by one 
			avercorr_open.close()
	    	    datecurr = datestart
	    plt.gcf().autofmt_xdate() 
	    plt.show()
	    
	    print "y is", y 

	def replace(self,a,b,val,replace_val): 
		print "\n a",a
		print "\n b",b
		print "\n val",val
		print "\n replace_val", replace_val
		machan = numpy.where(a == float(val))
		print "\n machan", machan
		x_coordinate = machan[0]
		print "\n x coordinate a", x_coordinate
		for index in range(0, len(x_coordinate)):
			x = x_coordinate[index]
			if not  x > (len(b)-1):
				b[x] = float(replace_val)
		return b
	def replacev2(self,a,b,val,replace_val): 
		print "\n a",a
		print "\n b",b
		print "\n val",val
		print "\n replace_val", replace_val
		machan = numpy.where(a < float(val))
		print "\n machan", machan
		x_coordinate = machan[0]
		print "\n x coordinate a", x_coordinate
		for index in range(0, len(x_coordinate)):
			x = x_coordinate[index]
			if not  x > (len(b)-1):
				b[x] = float(replace_val)
		return b

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
#    Timeseries().calccorrelation(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
#    Timeseries().daycorrelation(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[7])
#    Timeseries().anoncorrelation(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
    Timeseries().plotcorrelation(sys.argv[1], sys.argv[2], sys.argv[6])
