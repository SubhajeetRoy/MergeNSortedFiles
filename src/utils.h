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

#ifndef LINUX_FILE_API_UTILLS___
#define LINUX_FILE_API_UTILLS___
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "stdint.h"
#include <string>
#include <sstream>
#include <ctime>
#include <vector>

class FileInfo
{
public:
    FileInfo(std::string name, bool isTmpFile = false)
    {
        m_Filename.assign(name);
        m_isTmpFile = isTmpFile;
    }
    bool m_isTmpFile;
    std::string m_Filename;
};
class OsUtils
{
public:
    OsUtils(){};
    virtual ~OsUtils(){

    };
    virtual bool ParseContents(std::string &FileContents, std::tm &T, time_t &Epoch, int &Millis,std::string &symbol) = 0;
    virtual int DeleteFile(const char *FileName) = 0;
    virtual bool GetFiles(char *Path, std::vector<FileInfo *> &Out) = 0;
    virtual FILE *OpenFile(char *FileName, char *Mode) = 0;
};
class LinuxUtilsImpl : public OsUtils
{
public:
    LinuxUtilsImpl() {}
    virtual ~LinuxUtilsImpl() {}
    virtual int DeleteFile(const char *FileName);
    virtual bool GetFiles(char *Path, std::vector<FileInfo *> &Out);
    virtual FILE *OpenFile(char *FileName, char *mode);
    virtual bool ParseContents(std::string &FileContents, std::tm &T, time_t &Epoch, int &Millis, std::string &symbol);
};

#endif
