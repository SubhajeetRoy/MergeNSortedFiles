/*******************************************************************

 Copyright 2022 Subhajeet Roy

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
********************************************************************/
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"
#include <bits/stdc++.h>

using namespace std;

/**
 * deletes a file
 * @param[out] FileName : file to open
 * @return success/failure.
 */

int LinuxUtilsImpl::DeleteFile(const char *FileName)
{
    int ret = 0;

    ret = remove(FileName);

    if (ret == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }

    return (0);
}
/**
 * Opens a file
 *
 * @param[in] FileName : file to open
 * @param[in] mode : mode to open
 * @return file pointer.
 */

FILE *LinuxUtilsImpl::OpenFile(char *FileName, char *Mode)
{
    FILE *fp = fopen(FileName, Mode);
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}
/**
 * Parse the contents from a read line from a file.
 *
 * @param[out] t : time structure
 * @param[out] epoch : num seconds  from epoch
 * @param[out] millis : milliseconds
 * @param[in] FileContents :string containing the full line 
 * @param[in] FirstBatch : indicates if its first batch of input files or tmp output files
 * @return success or failure.
 */

bool LinuxUtilsImpl::ParseContents(std::string &FileContents, std::tm &T, time_t &Epoch, int &Millis, std::string &symbol)
{

    std::istringstream strm(FileContents.c_str());
    std::string datecol, timecol;
    if (!symbol.empty())
    {
        //in first run the date is in first col and time in seconds.
        strm >> datecol;
        strm >> timecol;
    }
    else
    {
        //in secon run onwards the date is in second col and time in third.
        string tmp;
        // strm >> symbol;
        strm >> tmp;
        strm >> datecol;
        strm >> timecol;
    }

    timecol.pop_back();

    size_t found = timecol.find(".");

    if (found != string::npos)
    {

        std::string fullstring = datecol + "T" + timecol;

        std::string timestr = timecol.substr(0, found);
        std::string millistr = timecol.substr(found + 1, timecol.size());
        stringstream tmp(millistr);
        tmp >> Millis; //save millis

        std::istringstream ss(fullstring);
        if (ss >> std::get_time(&T, "%Y-%m-%dT%H:%M:%S"))
        {

            Epoch = std::mktime(&T); //save epoch
            return true;
        }
        else
        {
            std::cout << "Parse failed\n";
            return false;
        }
    }
    else
    {

        return false;
    }
    return false;
}
/**
 * Return all files in a given folder in a vector.
 *
 * @param[out] vector with absolute Path of the files
 * @param[in] Path :Path of the folder to scan
 * @return success or failure.
 */
bool LinuxUtilsImpl::GetFiles(char *Path, std::vector<FileInfo *> &Out)
{
    struct dirent **filelist;
    int num_files, i;
    num_files = scandir(Path, &filelist, 0, versionsort);
    if (num_files < 0)
    {
        printf("Error using scandir errno= %d \n", errno);
        return false;
    }
    else
    {
        for (i = 0; i < num_files; ++i)
        {
            if (strcmp(filelist[i]->d_name, ".") == 0 || strcmp(filelist[i]->d_name, "..") == 0)
            //ignore these 2 files
            {
                continue;
            }
            std::stringstream tmp;
            tmp << Path << "/" << filelist[i]->d_name;
            std::string absPath = tmp.str();
            FileInfo *FileInfo_ = new FileInfo(absPath);
            Out.push_back(FileInfo_);

            free(filelist[i]);
        }

        free(filelist);
    }
    return true;
}
