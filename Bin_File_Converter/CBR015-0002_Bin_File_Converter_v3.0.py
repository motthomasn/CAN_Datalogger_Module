################################################################################
##
## Copyright 2020 THOMAS NAUGHTON <motthomasn@gmail.com>
##  
## Permission is hereby granted, free of charge, to any person obtaining a copy 
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights 
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
## copies of the Software, and to permit persons to whom the Software is 
## furnished to do so, subject to the following conditions:
##  
## The above copyright notice and this permission notice shall be included in 
## all copies or substantial portions of the Software.
##  
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
## SOFTWARE.
## 
##
##
## CBR015-0002_BIN_FILE_CONVERTER
## Version 3.0.0  DD-03-2020
## 
## This code is designed to convert the binary log files produced by 
## CBR015-0002_CAN_DATALOGGER_ASSY_CODE_v2.X.X into .csv format files for 
## further analysis.
## The output file format is set specifically to comply with EFI Analytics 
## MegaLogViewer data analysis software. 
## 
## See CBR015-0002_CAN_DATALOGGER_ASSY documentation for further details & 
## overview
## 
##
################################################################################
## Revision History
##
## v1.0   - Original script for use with CBR015-0002 v1.0 code
## v1.1   - Updated script to work with CBR015-0002 v1.1 code
## v1.1.1 - Minor change to add conversion information output to terminal
## v2.0.0 - Major change. Read dat file to memory in one go as single bytes to 
##          save time and convert data afterwards
## v2.0.1 - Minor change to revise terminal output summary information
## v2.1.0 - Updated script to work with CBR015-0002 v1.2 code
## v2.1.1 - Minor change to write conversion summary information to .xlsx file 
##          along with file comment to use as file index. Track choice added to 
##          input to allow calculation of lap times from data captured at track. 
##          Track start/finish lines need to be manually entered in config file.
##          Error checks added to user input stage
## v3.0	  - Major change.
##          Initial python version
##          Uses CAN .dbc file to decode binary data
##          Works with specific new Google Drive folder format
##          Provisions for track timing included but timing not actually carried out  
##
################################################################################

# import required tools first
import cantools
import easygui
import csv
import time
import os
from pathlib import Path
import pandas
import sys


    
###############################################################################
# First define paths to all required files
# This script returns a list of file paths which do not have corresponding files in another directory
# it requires a specific folder structure as follows:
#                   Base
#                   / \                     \
#     Processed_Data   Scripts Folder       Config Files Folder
#               /       \
#       Raw_Data        Script Location
#
# the script is a tester specifically for the CBR250RR datalogger conversion
#
# Arguments:
#   - RawDataPath:  Output. String path to folder containing raw .dat files
#   - ProcDataPath:  Output. String path to folder containing processed .csv files
#   - RawDataPath:  Output. String path to folder containing raw .dat files
#   - ConfigPath:   Output. String path to folder containing all config files required for conversion
#   - IndexFile:    Output. String path to index file
#
#
# get the path from which this script is being executed
# this is to avoid errors if the folder locations vary from machine to machine
ScriptPath = os.path.dirname(os.path.realpath(__file__))

# Now go two levels up to get the base path
BasePath = str(Path(ScriptPath).parents[1]) # str() is required otherwise a windows path object is returned

ProcDataRelPath = '/03_Recorder_Data' # path to data location relative to script location

ProcDataPath = BasePath + ProcDataRelPath

RawDataPath = ProcDataPath + '/Raw_Data'

ConfigPath = BasePath + '/02_Config_files'

dbcConfig = ConfigPath + '/CBR015-0002_CAN_LOGGER_CONFIG.dbc'

TrackConfig = ConfigPath + '/Track_List.csv'

IndexFile = ProcDataPath + '/00_CBR250RRi_DataLog_Index.csv'


###############################################################################
## Get the names of files that need to be converted ##

# create 2 lists of processed & raw data without file extensions for comparison
ProcessedFiles = []
RawFiles = []

# Get the names of processed files and add them to a list
with os.scandir( ProcDataPath ) as entries:
    for entry in entries:
        if entry.is_file(): # check if the entry is a file and not a directory
            ProcessedFiles.append( entry.name.replace('.csv', '') ) # remove the extension from the file name and add it to the list


# Do the same with raw files
with os.scandir(RawDataPath) as entries:
    for entry in entries:
        if entry.is_file(): # check if the entry is a file and not a directory
            RawFiles.append( entry.name.replace('.dat', '') ) # remove the extension from the file name and add it to the list


# compare the two lists and extract the filenames which have not been processed            
NewFiles = []
NewFilesPath = []
Tracks = []
Date = []

for name in RawFiles:
    if name not in ProcessedFiles:
        NewFiles.append( name + '.csv') # create a seperate list with only output names for future use
        NewFilesPath.append(RawDataPath + '/' + name + '.dat') # if the file doesnt exist in processed list it is new. Add path and file extension
        
        # get the date or record from filename. Fixed file name format allows this to be hardcoded
        Year = name[10:14]
        Month = name[14:16]
        Day = name[16:18]
        Date.append( Year + '-' + Month + '-' + Day ) # create a date string in format YYYY-MM-DD
        
        if len(name) > 24:
            # File includes track data. Base filename from longer is 24 characters long. CBR250RRi_YYYYMMDDhhmmss_TrackName.dat
            Tracks.append( name[ -( len(name) - 25 ): ] ) # due to fixed length base filename, can extract track name from end 
        else:
            Tracks.append('None')


# carry out a check here if any names were passed to the unprocessed files list
# if no new files were found, report to user and terminate script
if NewFilesPath == []:
    easygui.msgbox( msg='No new files found!', title="INFO" )
    sys.exit()
    

# Create a dataframe to contain file & conversion information    
TrackAssoc = dict( zip( NewFilesPath, Tracks ) ) 

FileInfo = pandas.DataFrame.from_dict( TrackAssoc, orient='index', columns=['Track'] )  # create a dataframe from dictionary

FileInfo['FileName'] = NewFiles # add a column to the dataframe for name display only

FileInfo['Date'] = Date # add comments to dataframe

FileInfo = FileInfo[ ['FileName', 'Date', 'Track' ] ] # reorder the dataframe to match index output

FileInfo['NumLaps'] = ['NaN'] * len( NewFiles ) # add a column with values NaN. Placeholder for future script revision

FileInfo['FastestLap'] = ['NaN'] * len( NewFiles ) # add a column with values NaN. Placeholder for future script revision

Comment = easygui.multenterbox( 'Enter comments to help describe data file content', 'Comments', NewFiles )

FileInfo['Comment'] = Comment # add comments to dataframe

FileInfo['LogLength'] = ['NaN'] * len( NewFiles ) # add a column with values NaN. Will be populated later

FileInfo['InFileSize'] = ['NaN'] * len( NewFiles ) # add a column with values NaN. Will be populated later

FileInfo['ConversionTime'] = ['NaN'] * len( NewFiles ) # add a column with values NaN. Will be populated later

FileInfo['ConvRateSize'] = ['NaN'] * len( NewFiles ) # add a column with values NaN. Will be populated later

FileInfo['ConvRateTime'] = ['NaN'] * len( NewFiles ) # add a column with values NaN. Will be populated later



###############################################################################
# read the track start/finish data from a csv into a dataframe indexed by track name
TrackData = pandas.read_csv( TrackConfig, index_col='Track_Name') 



###############################################################################
# setup DBC config

# first load the dbc file
db = cantools.database.load_file( dbcConfig ) # returns Database class

MsgList = db.messages # get a list of the messages in the dbc file

IDList = [] # create an empty list for the IDs

# create a list of just IDs
for i in range(len(MsgList)):
    IDList.append(MsgList[i].frame_id)
    
IDList.sort() # Sort the ID list in ascending order


# Now we need to extract all the signal names & units and put them in individual lists
UnitList = [] # create an empty list to be populated later
NameList = [] # create an empty list to be populated later

for i in range( len( IDList ) ):
    
    temp_msg = db.get_message_by_frame_id( IDList[i] ) # returns Message class
    temp_sigs = temp_msg.signals
    
    for j in range( len( temp_sigs ) ):
        
        NameList.append( temp_sigs[j].name ) 
        UnitList.append( temp_sigs[j].unit ) # append a unit name at each iteration
           

# Now create a master data dictionary which will be updated as messages are decoded
# First we need a data list to form our values. These can be zero for now
NullData = [0] * len( NameList ) # create a list of zeros equal to the number of signals. The zeros will be replaced later

DataMaster = dict( zip( NameList, NullData ) )


###############################################################################
# Load each data data file in turn and create a new file for saving to

for InputFile in NewFilesPath:

    # get data for conversion stats
    start_time = time.perf_counter() # get a start time for the conversion
    
    
    # manipulate the input file path to create the output file path
    OutputFile = InputFile.replace( RawDataPath, ProcDataPath ) # replace the main path part
    OutputFile = OutputFile.replace( '.dat', '.csv' ) # replace file extension. Same base name
    
    
    # Create the output file and write headers
    with open( OutputFile, 'a+', newline='' ) as outfile:
        writer = csv.writer( outfile )
        writer.writerow( NameList )
        writer.writerow( UnitList )
        
    
    # Open the input file and read all the data into the script then close it
    with open( InputFile, 'rb' ) as infile:
        RawData = infile.read() # reads the entire file in one hit
        
    
    ###############################################################################
    # Loop through the binary data, 8 bytes at a time, decode the data and write decoded data to csv one line at a time
    
    # Need to open the output file and keep it open for the loop
    with open( OutputFile, 'a+', newline='' ) as outfile:
        writer = csv.writer( outfile )
        
        num_bytes = len( RawData ) # get total bytes in raw data this will be used as a hard stop for the loop
        num_chunks = len( IDList ) # this is the total number of 8 byte chunks being logged. i.e. one line of data
        
        j = 0 # initialise the ID_List loop index
        
        # maybe while Raw_Data: may be cleaner here?
        for i in range( 8, num_bytes, 8 ): 
            # range(start, stop, step)
            
            start_byte = ( i - 8 )
            end_byte = i
            
            # decode 8 bytes according to dbc
            decoded_temp = db.decode_message( IDList[j], RawData[start_byte:end_byte] )
            
            # update the master data dictionary with the newly decoded data
            DataMaster.update( decoded_temp )
            
            j += 1 # increment j by 1
            
            if j == num_chunks:
                # means that we have reached the end of the data line
                
                data_line = DataMaster.values() # create a list from the master data values
                writer.writerow(data_line) # write the data list to output file
                
                j = 0 # reset j



    ###########################################################################
    # Assemble data to write to index file
    
    end_time = time.perf_counter() # get the end time for the conversion  
    conv_time = end_time - start_time
    file_stats = os.stat( InputFile ) 
    InputFile_Size = ( file_stats.st_size / 1024 ) # input file size in KB
    Log_Length = DataMaster['Time'] # grab the last datapoint (s)
    conv_rate_size = InputFile_Size / conv_time # conversion rate in KB/s
    conv_rate_time = Log_Length / conv_time # conversion rate in s/s
    
    # Add the data to the FileInfo dataframe
    #FileInfo.at[ InputFile, 'NumLaps'] = Placeholder
    #FileInfo.at[ InputFile, 'FastestLap'] = Placeholder
    FileInfo.at[ InputFile, 'LogLength'] = Log_Length
    FileInfo.at[ InputFile, 'InFileSize'] = InputFile_Size
    FileInfo.at[ InputFile, 'ConversionTime'] = conv_time
    FileInfo.at[ InputFile, 'ConvRateSize'] = conv_rate_size
    FileInfo.at[ InputFile, 'ConvRateTime'] = conv_rate_time

    
    
# Write the conversion info to the index file
with open( IndexFile, 'a+', newline='' ) as file:
    writer = csv.writer( file )
    for index, row in FileInfo.iterrows():
        writer.writerow( list( FileInfo.loc[index] ) )
        #temp_data = pandas.Series(row).values # alternative method
        #writer.writerow( temp_data )

# Open messgae box to inform user process completed
easygui.msgbox( msg=( 'File Conversion Successful! \n\n' + str( len(NewFiles) ) + 'off files converted'), title="INFO" )