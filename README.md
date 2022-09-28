# Merge N-Sorted files 

## Problem defination

If there are N-Sorted files as per timestamp, the problem is to achieve a resulting sorted output  file using a C++ program as efficiently as possible .As there can be many files to be read and each file can have more than 100000 entries put together cab run into billions and inefficient handling can cause the program to run into days.Also some OS like Windows there is a hard restriction  on the number of files that a program can open simultaneously. Moreover , bringing the entire data to memory simultaneously is not viable .

## Brief Explanation 

Implementation of Source code to Merge N-Sorted files to single sorted Output file.

- The code assumes the input files are all sorted as per timestamps of each line .

- On First run, the symbol name is taken from filenames and time is calculated as seconds from epoch plus the millis

- The code merges the files in batches and creates a tmp output file after each successful run.

- On Second run the  tmp output files are again merged further into a smaller set of tmp output files.

  **The process repeats till there is only one output file left with Sorted Merged Contents**

-  Size of each batch can be adjusted using a macro as explained below and can be adjusted as per the OS .

- Number of open files at a given time are never more than the batch size .

- Number of lines read  from the files into the  heap at a given time is never  more than the batch size .

- During each batch run only the lines corresponding to row number is read from each file and stored in heap and the minimum of these entries is written to the output file .

- The code uses a recursive external merge sort to sort the N-Sorted files accoring to seconds from epoch and millis .if timestamp is same then alphabetical order of symbol is used

*Please review the code for more details on the algorithms*

**Build Requirements**

```

OS - Ubuntu 18.04LTS
Install 
apt update
apt install build-essential cmake g++

```

Even though the application is built for Linux , it can be easily ported to Windows by deriving and implementing class OSUtils which implements the APIs interacting with the Operating system.

**Customisation**

```
In coreworker.h Change below if needed
//path to the folder containing sorted input files
#define FOLDER_PATH "./data"

//Batch size to read num files at a time and sort data to output file
#define ALLOWED_BATCH_SIZE 50

```

**Compile and run**

```
# in the project dire
cmake CMakeList.txt
cmake
make 
./MAV
```

References

- https://en.wikipedia.org/wiki/External_sorting
- http://web.eecs.utk.edu/~leparker/Courses/CS302-Fall06/Notes/external-sorting2.html