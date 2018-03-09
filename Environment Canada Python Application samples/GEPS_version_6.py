#!/usr/bin/python
# GEPSEnsMean12hAcc.py

# Written by Kapilan Satkunanathan and Dorothy Dunford, September 2017
# based on code by Dorothy Dunford: PlotWatersheds.py
# and Benoit Lapointe: strfw_compare_daily_statsonly.py


#####

# This function performs a verification of mean runoff for each launchtimes simulated using the GEPS forecast
# The analysis and all forecast hours are valid at the same date/time; the start of the forecast is earlier when considering the later forecast hours.
# e.g., when considering 2015110500, the simulated fields considered are:
# forecasts: 2015110500_024_GrLksStLR, 2015110500_048_GrLksStLR, 2015110500_072_GrLksStLR, 2015110500_096_GrLksStLR, 2015110500_120_GrLksStLR, 2015110500_144_GrLksStLR, 2015110500_168_GrLksStLR, 2015110500_192_GrLksStLR, 2015110500_216_GrLksStLR, 2015110500_240_GrLksStLR

#####

# The function is called with the following arguments:
# Compulsory arguments:
# arg1: the start date of the analysis period in the following format yyyymmdd
# arg2: the end date of the analysis period in the following format yyyymmdd
# arg3: path of directory to receive generated files the output files: e.g. /cnfs/dev/cmd/cmde/afsgdod/Maestro/watroute/input/GrLksStLR/constant/geophys/
# arg4: path where GEPS standard files are kept, e.g. GrLksStLR

# To run this script from the command line, open the run_nameofscript.py  using vim:
# insert desired parameters
# make parent script(run_nameofscript.py) executable using chmod u+x 
# to adjust more specific parameters look at the beginning of the script and make adjustments to intial variables if necessary
# take a look at the cooments to get a ovierview of how the script works
#####

# Notes:


######################################################################

import csv                                                           # Import modules
import datetime
import os
import string
import sys
import numpy
import shutil
import math
import rpnpy.librmn.all as rmn                                       # For Python RPN v2.0


class FloodPredictorMachine:
    def compare(self, datestr_str, dateend_str, dir_out, in_dir, gridtemplate):
    
        #Intializing some variables
        datestart = datetime.datetime.strptime(datestr_str, "%Y%m%d")# start date
        datecurr = datetime.datetime.strptime(datestr_str, "%Y%m%d") # current date
        dateend = datetime.datetime.strptime(dateend_str, "%Y%m%d")  # end date
	datemark_str = "20160602"
	datemark = datetime.datetime.strptime(datemark_str, "%Y%m%d")# to calibrate normand date in the beginning
	dateinit_str = "20160526"				     # dateinit set normand date to this if date norm is less than datemark
	dateinit= datetime.datetime.strptime(dateinit_str, "%Y%m%d") # dateinit set normand date to this if date norm is less than datemark
        missing_val = -999.999                                       # The missing data value
        acchrs = [24]                                                # The length of the accumulation periods to be processed
        nacchrs = len(acchrs)                                        # size of acculmulation hours 
        accint = [24]                                                # all the acculmulation hours increments  we are going to verify
        syntimes=[0,12]                                              # launch times
        Nsyntimes = len(syntimes)                                    # amount of launchtimes
#        Flast = 48                                                  # The last forecast hour to be processed
        Flast = 240                                                  # The last forecast hour to be processed
        numdates = (((dateend - datestart).days) + 1)                # The number of dates to be processed: Nsyntimes synoptic times per day, including on date_start and date_end
        field = "N0"                                                 # field to read and calculate mean of 
#        nmem = 3                                                    # number of members
        nmem = 20                                                    # number of members
	nmem_reset =20						     # keep same as number of members
        record = 'X'                                                 # IDK
    	normanddir = "/users/dor/afsg/kas/diskspace/normand"	     # directory to files where normand files are kepti
	logic = True						     # used to get the  right normand file associated with the GEPS file
	twelvelaunchtime_incrementation = [12,36,60,84,108,132,156,180,204,228]	 # used for forecast in the 12h launchtimes loop
        a_index = 100						     # used to see what is happening to  a point in array  x coordinate in array
	b_index = 150						     # used to see what is happening to  a point in array  y coordinate in array
	missing_files_list = []
#######

        # Get the field dimensions
        infle = "%04d%02d%02d%02d_%03d_%03d"%(datecurr.year, datecurr.month, datecurr.day, syntimes[0], 24 ,001) # creating the file name to be read from
        inpath = os.path.join(in_dir, infle)                         # join path to directory and name of files we want to open
        fst_rec = self.ReadFld(inpath, field, 24, record)            # Read the data and metadata of the watersheds from the shed file 
        ni = fst_rec['ni']
        nj = fst_rec['nj']


    
        # Check that the directory to receive the output files exists. If it doesn't, create it recursively
        if not os.path.exists(dir_out):
            try:
                os.makedirs(dir_out)
            except:
                print 'The directory to receive the output files does not exist and cannot be created recursively'
                quit()


    
        # Checking if start date is not greater than end date
        if dateend < datestart:
          print 'ERROR: the end date', dateend.strftime("%Y%m%d%H"), 'is earlier than the start date', datestart.strftime("%Y%m%d%H")
          print 'Aborting'
          quit()

###################################
	n = 0							     # used to get corresponding normand file with the GEPS file
        # Code to calculate mean starts here 
        for accrhrs_index in range(0,nacchrs):                       # loop for each outputs          

            for days in range(0,numdates):                           # loop for number of days

                for launchtimes in range(0,Nsyntimes):               # loop for amount of launch times
                    out_fle="%04d%02d%02d%02d"%(datecurr.year , datecurr.month , datecurr.day  ,syntimes[launchtimes])
                    print '\n outfile is: ', out_fle
                    outpath = os.path.join(dir_out,out_fle)          # creating the standard file format name that is to be written to 
                    oldfld = numpy.zeros([ni,nj], dtype=numpy.float32, order='FORTRAN')

		    if launchtimes == 0:
			    for forecast in range(0,Flast+1,accint[accrhrs_index]):  # loop for forecast hour
				tree_d_array = numpy.zeros([ni,nj], dtype=numpy.float32, order='FORTRAN')
				nmem = nmem_reset
				for member in range(1,nmem+1):               # loop for ensemble members excludes control member

				    x=int(math.ceil(forecast/24)*24)											     # this is used to get the forecast hour which is included in the file's name  
				    print "x is ",x
				    infle = "%04d%02d%02d%02d_%03d_%03d"%(datecurr.year , datecurr.month , datecurr.day  ,syntimes[launchtimes] , x ,member) # creating the file name to be read from
				    print '\n',infle
				    inpath = os.path.join(in_dir, infle)     										     # join path to directory and name of files we want to open

				    ########this part has been hard coded for missing GEPS files #####################################################################################################################
				    print '\n is file not missing? : ',(os.path.isfile(os.path.join(in_dir, infle)))   
				    #print (os.path.isfile(os.path.join(in_dir, "%04d%02d%02d%02d_%03d_%03d"%(2016 , 06 , 18  , 0  , 72 , 19))))   
				    #print (os.path.isfile(os.path.join(in_dir, "%04d%02d%02d%02d_%03d_%03d"%(2016 , 06 , 18  , 0  , 72 , 18))))   
				    if os.path.isfile(os.path.join(in_dir, infle)) == False:
				    	nmem = nmem -1
					print '\n file that is  missing:', infle
					missing_files_list.append(infle)
					print '\n list so far is: ', missing_files_list 
				        
				    else:
				    #################### hard coding end ##########################################################################################################################
				    	print '\napples: ',inpath,'mango: ',field,'fore: ',forecast,'rec: ',record						     # shows what is being read 
				    	fst_rec = self.ReadFld(inpath, field,forecast,record)  								     # Read the data and metadata of the watersheds from the shed file 
				    	newfld = fst_rec['d']

				    	if forecast == 0:													     # sum up the twenty member array in an array
						oldfld = numpy.add(newfld,oldfld)
				    	else:
						tree_d_array = numpy.add(newfld,tree_d_array)

				if forecast == 0:  

				    oldfld = oldfld / nmem												     # divide by number of members to get average special case for initial 
				    shutil.copy2(os.path.join(dir_out, gridtemplate), outpath) 								     # complete target filename given and copy the template grid file to it

				else:
				    # this process takes the average of the array by diving by the aamount of membets in addition subtracts it by the previous average array also this new array is stored as the previous array 
				    print '\n',"old test b4:", oldfld[a_index][b_index]
				    newfld = tree_d_array/nmem
				    temp =newfld
				    print '\n',"before subtraction", newfld[a_index][b_index]
				    newfld = numpy.subtract(newfld,oldfld)
				    print '\n',"after  subtraction", newfld[a_index][b_index],
				    oldfld = temp
				    print "min is",numpy.amin(newfld),"max is" ,numpy.amax(newfld), "index of min", numpy.where(newfld == numpy.min(newfld)),"index of max", numpy.where(newfld == numpy.max(newfld)) 
				   
				    # this following logic is to configure the normand files if date curr is before date mark then goes to intial date if not then goes to the next week of date init 
				    if n % 7 == 0 or logic == True:
					datenorm = datecurr
					print 'Sri'
					if datenorm < datemark and logic == True:
						datenorm = dateinit
						print 'Lanka'
					elif datenorm == datemark and logic == True:
						logic = False
						n = 0 
						'is Great'


				    # basically minusing mean and dividing by standard deviation note standard deviation has lower threshold in order to avoid cases like 0/0 and 1/0 and those values will be reduced to zero instead
				    print 'date norm is', datenorm, 'datenorm.year',  datenorm.year, 'month',datenorm.month,'day',datenorm.day ,'syntimes', syntimes[0] , 'x is',x
				    normandfle = "%04d%02d%02d%02d_%03d"%(datenorm.year , datenorm.month , datenorm.day , syntimes[0] ,x)
				    print 'ORIG normand',datenorm
#############################################################################33
				    dateoriginal = datenorm
				    datetemp = datenorm
				    datetemp_2 = datenorm
				    datetemp_2 += datetime.timedelta(days=14)  
				    datetemp_3 = datenorm
				    print '1 normand',datenorm
				    datetemp_3 += datetime.timedelta(days=7)
				    datetemp_4 = datecurr
				    datetemp_4 += datetime.timedelta(hours=(x-24))  
				    print 'before normand',datenorm
				    if (datetemp_4 >= datetemp_2):
				    	datenorm = datetemp_2
				    elif (datetemp_4 >= datetemp_3):
					datenorm = datetemp_3
				    normandfle = "%04d%02d%02d%02d_%03d"%(datenorm.year , datenorm.month , datenorm.day , syntimes[0] ,x)
				    print 'after normand',datenorm
#####################################################################################################3
				    ######################################################################3####### hard code #######################################################################################################
				    if datenorm.month == 7 and datenorm.day == 7:
				    	normandfle = "%04d%02d%02d%02d_%03d"%(datenorm.year , datenorm.month , 14 , syntimes[0] ,x)
				    ############################################################################## hard code ########################################################################################################
				    print'\n normand file is ', normandfle
				    normandpath = os.path.join(normanddir,normandfle)
				    norm_rec_mean= self.ReadFld_etiket(normandpath, field, forecast,'ECMEAN__NALL',record)
				    norm_mean_fld = norm_rec_mean['d']
				    print '\n',"normand mean", norm_mean_fld[a_index][b_index]
				    norm_rec_std = self.ReadFld_etiket(normandpath, field, forecast,'ECSSTD__NALL',record)
				    norm_std_fld = norm_rec_std['d']
			            machan = numpy.where(norm_std_fld <= 0.01)
				    print'\n machan use this', machan
				    print machan[0], 'list seperate', machan[1]
				    x_coordinate = machan[0]
				    y_coordinate = machan[1]
				    print '\n',"normand standard deviation", norm_std_fld[a_index][b_index]
				    newfld = numpy.subtract(newfld, norm_mean_fld)
				    newfld = numpy.divide(newfld, norm_std_fld)
				    print newfld
				    for index in range(0, len(x_coordinate)):
				    	x = x_coordinate[index]
				        y = y_coordinate[index]
					newfld[x][y] = missing_val
				    print '\n',"anonmaly", newfld[a_index][b_index]
				    print newfld
				    print "min is",numpy.amin(newfld),"max is" ,numpy.amax(newfld), "index of min", numpy.where(newfld == numpy.min(newfld))
				    fst_rec.update({'d':numpy.asfortranarray(newfld),'ip2':forecast, 'ip3':forecast-24, 'etiket':'GEPSNORMANOM'})
				    print outpath  
				    self.WriteFld(outpath,fst_rec)
				    datenorm = dateoriginal
		    #literally the code below is the same as one above except this has an offset of 12 hours for the forecast look for comment lines with apples that will show you the difference
		    if launchtimes == 1:

			    for forecast_index in range(0,len(twelvelaunchtime_incrementation)):  # apples loop for forecast hour
				forecast = twelvelaunchtime_incrementation[forecast_index]        # apples
				tree_d_array = numpy.zeros([ni,nj], dtype=numpy.float32, order='FORTRAN')
				nmem = nmem_reset
				for member in range(1,nmem+1):               # loop for ensemble members excludes control member

				    x=int(math.ceil(forecast/24.0)*24)
				    infle = "%04d%02d%02d%02d_%03d_%03d"%(datecurr.year , datecurr.month , datecurr.day  ,syntimes[launchtimes] , x ,member) # creating the file name to be read from
				    print '\n',infle
				    inpath = os.path.join(in_dir, infle)     # join path to directory and name of files we want to open

				    ########this part has been hard coded for missing GEPS files #####################################################################################################################
				    print '\n is file not missing? : ',(os.path.isfile(os.path.join(in_dir, infle)))   
				    #print (os.path.isfile(os.path.join(in_dir, "%04d%02d%02d%02d_%03d_%03d"%(2016 , 06 , 18  , 0  , 72 , 19))))   
				    #print (os.path.isfile(os.path.join(in_dir, "%04d%02d%02d%02d_%03d_%03d"%(2016 , 06 , 18  , 0  , 72 , 18))))   
				    if os.path.isfile(os.path.join(in_dir, infle)) == False:
				    	nmem = nmem -1
					print '\n file that is missing:', infle
					missing_files_list.append(infle)
					print '\n list so far is: ', missing_files_list 
				    else:
				    #################### hard coding end ##########################################################################################################################
				    	print '\n apples: ',inpath,'mango: ',field,'fore: ',forecast,'rec: ',record
				    	fst_rec = self.ReadFld(inpath, field,forecast,record)  # Read the data and metadata of the watersheds from the shed file 
				    	newfld = fst_rec['d']

				    	if forecast == 12:
						oldfld = numpy.add(newfld,oldfld)
				    	else:
						tree_d_array = numpy.add(newfld,tree_d_array)

				if forecast == 12:                                             #apples
				    oldfld = oldfld / nmem
				    shutil.copy2(os.path.join(dir_out, gridtemplate), outpath) # complete target filename given and copy the template grid file to it

				else:

				    print '\n',"old test b4:", oldfld[a_index][b_index]
				    newfld = tree_d_array/nmem
				    temp =newfld
				    print '\n',"before subtraction", newfld[a_index][b_index]
				    newfld = numpy.subtract(newfld,oldfld)
				    print '\n',"after  subtraction", newfld[a_index][b_index]
				    oldfld = temp
				    
				    print "min is",numpy.amin(newfld),"max is" ,numpy.amax(newfld), "index of min", numpy.where(newfld == numpy.min(newfld)), "index of max", numpy.where(newfld == numpy.max(newfld))

				    if n % 7 == 0 or logic == True:
					datenorm = datecurr
					if datenorm < datemark and logic == True:
						datenorm = dateinit
					elif datenorm == datemark and logic == True:
						logic = False
						n = 0 
						
						
				    normandfle = "%04d%02d%02d%02d_%03d"%(datenorm.year , datenorm.month , datenorm.day , syntimes[0] ,x)
#############################################################################33
				    dateoriginal = datenorm
				    datetemp = datenorm
				    datetemp_2 = datenorm
				    datetemp_2 += datetime.timedelta(days=14)  
				    datetemp_3 = datenorm
				    datetemp_3 += datetime.timedelta(days=7)
				    datetemp_4 = datecurr
				    datetemp_4 += datetime.timedelta(hours=(x-24))  
				    print 'before normand',datenorm
				    if (datetemp_4 >= datetemp_2):
				    	datenorm = datetemp_2
				    elif (datetemp_4 >= datetemp_3):
					datenorm = datetemp_3
				    normandfle = "%04d%02d%02d%02d_%03d"%(datenorm.year , datenorm.month , datenorm.day , syntimes[0] ,x)
#####################################################################################################3
				    print 'after normand' ,datenorm
				    ######################################################################3####### hard code #######################################################################################################
				    if datenorm.month == 7 and datenorm.day == 7:
				    	normandfle = "%04d%02d%02d%02d_%03d"%(datenorm.year , datenorm.month , 14 , syntimes[0] ,x)
				    ############################################################################## hard code #######################################################################################################
				    print '\n normand file is', normandfle
				    normandpath = os.path.join(normanddir,normandfle)
				    norm_rec = self.ReadFld_etiket(normandpath, field, x,'ECMEAN__NALL',record)
				    norm_mean_fld = norm_rec['d']
				    print '\n',"normand mean", norm_mean_fld[a_index][b_index]
				    norm_rec = self.ReadFld_etiket(normandpath, field, x,'ECSSTD__NALL',record)
				    norm_std_fld = norm_rec['d']
			            machan = numpy.where(norm_std_fld <= 0.01)
				    print'\n machan use this', machan
				    print machan[0], 'list seperate', machan[1]
				    x_coordinate = machan[0]
				    y_coordinate = machan[1]
				    print '\n',"normand standard deviation", norm_std_fld[a_index][b_index]
				    newfld = numpy.subtract(newfld, norm_mean_fld)
				    newfld = numpy.divide(newfld, norm_std_fld)
				    for index in range(0, len(x_coordinate)):
				    	x = x_coordinate[index]
				        y = y_coordinate[index]
					newfld[x][y] = missing_val
   			            print newfld
				    print '\n',"anonmaly", newfld[a_index][b_index]
				    print newfld
				    print "min is", numpy.amin(newfld),"max is", numpy.amax(newfld), "index of min" ,numpy.where(newfld == numpy.min(newfld))
				    fst_rec.update({'d':numpy.asfortranarray(newfld),'ip2':forecast, 'ip3':forecast-24, 'etiket':'GEPSNORMANOM'})  
			            print outpath
				    self.WriteFld(outpath,fst_rec)
				    datenorm = dateoriginal
		n = n+1						     #used to configure appropriate normand file
                datecurr += datetime.timedelta(days=1)               #increment date by one 
        print '\n missing files are:', missing_files_list         
        print("\nend of main function")
        return None

########### User Defined  Methods ###################################################

    # Write a field
    def WriteFld(self,outfile, outrecord):
        filename = outfile
        if not os.path.isfile(filename):                                 # Check that the output file exists
            print 'The output file was not found:', filename
            quit()
    
        # Open the file
        if not rmn.isFST(filename):                                      # Check that the output file is a standard format file
            raise rmn.FSTDError("Not an FSTD file: %s " % filename)
        try:                                                             # Open the standard format file
            fstID = rmn.fstopenall(filename, rmn.FST_RW_OLD)
            print(fstID)
        except:
            raise rmn.FSTDError("File not found/readable: %s" % filename)

        # Write out the standard format record
        varname = outrecord['nomvar']
        try:
            rmn.fstecr(fstID,outrecord)
        except:
            raise rmn.FSTDError("Problem writing the record " + varname + " to file: %s" % filename)  # Issue a warning message

        rmn.fstcloseall(fstID)                                           # Close the standard format file

        return

######################################################################

    # Read a field
    def ReadFld(self, infile, field, fhr, fst_rec):
        filename = infile
        if not os.path.isfile(filename):                                 # Check that the input file exists
            print 'The input file was not found:', filename
            quit()

        # Open the file
        if not rmn.isFST(filename):                                      # Check that the input file is a standard format file
            raise rmn.FSTDError("Not an FSTD file: %s " % filename)
        try:                                                             # Open the standard format file
            fstID = rmn.fstopenall(filename, rmn.FST_RO)
        except:
            raise rmn.FSTDError("File not found/readable: %s" % filename)

        # Read in the sample field and metadata
        varname = field
        try:
            var_key = rmn.fstinf(fstID, ip2=fhr, nomvar=varname)['key']           # Find the 1st record with the matching name
        except:
            raise rmn.FSTDError("Problem searching for record " + varname + " in file: %s" % filename)  # Issue a warning message
        if var_key:                                                      # Process the current record
            try:
                fst_rec = rmn.fstluk(var_key)                            # Read the record

            except:
                raise rmn.FSTDError("Problem defining grid of file: %s" % filename)

        rmn.fstcloseall(fstID)                                           # Close the standard format file

        return fst_rec
###########################################################################
    # Read a field with etiket
    def ReadFld_etiket(self, infile, field, fhr,etik, fst_rec):
        filename = infile
        if not os.path.isfile(filename):                                 # Check that the input file exists
            print 'The input file was not found:', filename
            quit()

        # Open the file
        if not rmn.isFST(filename):                                      # Check that the input file is a standard format file
            raise rmn.FSTDError("Not an FSTD file: %s " % filename)
        try:                                                             # Open the standard format file
            fstID = rmn.fstopenall(filename, rmn.FST_RO)
        except:
            raise rmn.FSTDError("File not found/readable: %s" % filename)

        # Read in the sample field and metadata
        varname = field
        try:
            var_key = rmn.fstinf(fstID, ip2=fhr, nomvar=varname,etiket=etik )['key']           # Find the 1st record with the matching name
        except:
            raise rmn.FSTDError("Problem searching for record " + varname + " in file: %s" % filename)  # Issue a warning message
        if var_key:                                                      # Process the current record
            try:
                fst_rec = rmn.fstluk(var_key)                            # Read the record

            except:
                raise rmn.FSTDError("Problem defining grid of file: %s" % filename)

        rmn.fstcloseall(fstID)                                           # Close the standard format file

        return fst_rec
############################################ End of User Defined Methods  #######################################################

if __name__ == '__main__':
    if len(sys.argv) < 6:
        msg = "".join(("You need at least 5 arguments here and provided: {}\n\n",
                       "Correct usage is : {} datestr_str dateend_str dir_out\n\n",
                       "        datestr_str : start date for the verification yyyymmdd\n",
                       "        dateend_str : end date for the verification yyyymmdd\n",
                       "            dir_out : path of directory to receive the generated image\n",
                       "             in_dir : path of directory to standard files\n",
                       "       gridtemplate : template file containing navigation fields of model output\n",
                       ))
        raise TypeError(msg.format(len(sys.argv)-1, sys.argv[0]))
    FloodPredictorMachine().compare(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])

######################################################################

  

