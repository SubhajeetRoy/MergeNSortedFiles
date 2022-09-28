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
#include "coreworker.h"
#include "sortfiles.h"
using namespace std;

/*Single static instance of the core class */
CoreWorker *CoreWorker::s_Instance = NULL;

/*change below to the folder containing the  input files*/

#define FOLDER_PATH "./data"

/**
 * Initializes the class using 2 phase construction .
 *
 * @param values None.
 * @return  success or failed.
 */
bool CoreWorker::Init()
{
    //use Linux implementation for Ubuntu
    //for Windows implement separate Windows implementation of OsUtils
    if ((m_Utils = new LinuxUtilsImpl()) == NULL)
    {
        cout << "Failed to initialize CoreWorker" << endl;
        return false;
    }
    return true;
}
/**
 * Core entry function for sorting/merging files .
 *
 * @param values None.
 * @return  success or failed.
 */
bool CoreWorker::StartSolving()
{
    printf("%s\n", __PRETTY_FUNCTION__);
    std::vector<FileInfo *> fileinfo;
    //get all input file names in a given directory .
    //note its only queuring the inode table and not opening any files here .
    //for Windows use separate Windows implementation
    m_Utils->GetFiles((char *)FOLDER_PATH, fileinfo);
    long size = fileinfo.size();
#if DEBUG //print all the file names
    for (auto it = fileinfo.begin(); it != fileinfo.end(); ++it)
    {
        FileInfo *info = (*it);
        printf("File : %s %d\n", info->m_Filename.c_str(), info->m_isTmpFile);
    }
#endif

    SortFiles files(m_Utils);
    //start sorting
    files.StartSorting(fileinfo, size);

    fileinfo.clear();

    return 0;
}

CoreWorker *CoreWorker::getInstance()
{
    if (s_Instance == NULL)
    {
        s_Instance = new CoreWorker();
        if (s_Instance->Init() == false)
            return NULL;
    }
    return s_Instance;
}

int main()
{
    //create an instance of the main class to start working
    CoreWorker *worker = CoreWorker::getInstance();
    worker->StartSolving();
    return 0;
}
