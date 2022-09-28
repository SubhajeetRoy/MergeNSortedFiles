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
#ifndef ___COREWORKER_H
#define ___COREWORKER_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "stdint.h"
#include <string>
#include <sstream>
#include <vector>
#include "utils.h"

//path to the folder containing sorted input files
#define FOLDER_PATH "./data"


//Batch size to read num files at a time and sort data to output file
#define ALLOWED_BATCH_SIZE 2

class CoreWorker
{
public:
    bool StartSolving();
    static CoreWorker *getInstance();

private:
    bool Init();

private:
    static CoreWorker *s_Instance;
    OsUtils *m_Utils;
};

#endif
