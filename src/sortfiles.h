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
#ifndef SORT_UTILLS___
#define SORT_UTILLS___
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "stdint.h"
#include <string>
#include <sstream>
#include <vector>
#include "utils.h"
#include <bits/stdc++.h>

struct Element
{
	// The element to be stored
	time_t epoc_;
	int millis_;
	std::string line_;
	std::tm t_;
	int i;
	std::string symbol;
};
// A class for Min Heap
class MinElement
{
public:
	// pointer to array of elements in heap
	Element *elemArr;
	int heap_size;

public:
	MinElement(Element a[], int size);
	void MinElementify(int);

	// to get index of left child
	// of node at index i
	int left(int i) { return (2 * i + 1); }

	// to get index of right child
	// of node at index i
	int right(int i) { return (2 * i + 2); }

	// to get the root
	Element getMin() { return elemArr[0]; }

	// to replace root with new node Min Element
	void replaceMin(Element x)
	{
		elemArr[0] = x;
		MinElementify(0);
	}
};

class SortFiles
{
public:
	SortFiles(OsUtils *Utils) : m_Utils(Utils) {}
	int StartSorting(std::vector<FileInfo *> &fileinfo, long num_entries);

private:
	void BatchExternalMergeSortFiles(char *output_file, std::vector<FileInfo *> &fileinfo, int start, int end);

private:
	OsUtils *m_Utils;
};
#endif