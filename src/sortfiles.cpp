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

#include "sortfiles.h"
#include "coreworker.h"

using namespace std;

/**
 * function to swap two elements.
 * @param[in] x 
 * @param[in] y 
 */

void SwapNodes(Element *x, Element *y)
{
	Element temp = *x;
	*x = *y;
	*y = temp;
}

/**
 * Constructor: Builds a MinElement sorted arr from
// a given array a[] of given size
 * @param[in] x 
 * @param[in] y 
 */

MinElement::MinElement(Element a[], int size)
{
	heap_size = size;
	elemArr = a; // store address of array
	int i = (heap_size - 1) / 2;

	while (i >= 0)
	{
		MinElementify(i);
		i--;
	}
}

// A recursive method to sort Elements based on time

void MinElement::MinElementify(int i)
{
	int l = left(i);
	int r = right(i);

	int smallest = i;
	if (l < heap_size)
	{
		if (elemArr[l].epoc_ < elemArr[i].epoc_)
			smallest = l;
		if (elemArr[l].epoc_ == elemArr[smallest].epoc_)
		{
			if (elemArr[l].millis_ < elemArr[smallest].millis_)
			{
				smallest = l;
			}
			if (elemArr[l].millis_ == elemArr[smallest].millis_)
			{
				if (elemArr[l].symbol.compare(elemArr[smallest].symbol) < 0)
				{
					smallest = l;
				}
			}
		}
	}
	if (r < heap_size)

	{
		if (elemArr[r].epoc_ < elemArr[smallest].epoc_)
			smallest = r;
		if (elemArr[r].epoc_ == elemArr[smallest].epoc_)
		{
			if (elemArr[r].millis_ < elemArr[smallest].millis_)
			{
				smallest = r;
			}
			if (elemArr[r].millis_ == elemArr[smallest].millis_)
			{
				if (elemArr[r].symbol.compare(elemArr[smallest].symbol) < 0)
				{
					smallest = r;
				}
			}
		}
	}

	if (smallest != i)
	{
		SwapNodes(&elemArr[i], &elemArr[smallest]);
		MinElementify(smallest);
	}
}
/**
 * Sort N files starting from index=start and end. its assumed that each individual files are already sorted .
 * @param[in] char *output_file output file name
 * @param[in] fileinfo input filenames vector 
 * @param[in] start index from which start mergeing files in fileinfo  
 * @param[in] end index till  which continue mergeing files in fileinfo  
 */

void SortFiles::BatchExternalMergeSortFiles(char *output_file, std::vector<FileInfo *> &fileinfo, int start, int end)
{
	int num_partitions = end - start;

	FILE *in[num_partitions];

	string inputnames[num_partitions];
	int k = start;

	for (int i = 0; i < num_partitions; i++)
	{
		size_t found = fileinfo[k]->m_Filename.find(".txt");
		//check to see if we are in the First Run where symbol name has to be read from filename
		if (found != string::npos)
		{
			printf("found =%d \n", found);
			std::string symbolname = fileinfo[k]->m_Filename.substr(0, found);
			symbolname = symbolname.substr(symbolname.find("./data/") + 7, symbolname.size());
			inputnames[i].assign((char *)symbolname.c_str());
			printf("symbol =%s \n", symbolname.c_str());
		}

		// Open input files in read mode.
		in[i] = m_Utils->OpenFile((char *)fileinfo[k++]->m_Filename.c_str(), (char *)"r");
	}

	// FINAL OUTPUT FILE
	FILE *out = m_Utils->OpenFile((char *)output_file, (char *)"w");

	// Create a  array of Element Objects. Every Element has first line of the input file.
	Element *elemArr = new Element[num_partitions];
	int i;
	bool isSymbolInFileName = false;
	//loop through all the input files in the given lot
	for (i = 0; i < num_partitions; i++)
	{
		char file_contents[1024];
		//get the first line of the input file
		fscanf(in[i], "%[^\n] ", file_contents);
		elemArr[i].line_.assign(file_contents);

		if (!inputnames[i].empty())
		{
			elemArr[i].symbol.assign(inputnames[i].c_str()); //save the symbol name
			isSymbolInFileName = true;
		}

		//assign the contents of the first line to Element class in ith position
		while (m_Utils->ParseContents(elemArr[i].line_, elemArr[i].t_, elemArr[i].epoc_, elemArr[i].millis_, inputnames[i]) == false)
		{
			//its possible that the first line contain only the headers and parsing time component will fail ,in such case read the next line
			//for a valid line
			fscanf(in[i], "%[^\n] ", file_contents);
			elemArr[i].line_.assign(file_contents);
		}

		// Index of the input file
		elemArr[i].i = i;
	}

	MinElement hp(elemArr, i);

	int count = 0;
	fprintf(out, "Symbol ,%s\n", "Timestamp, Price, Size, Exchange, Type"); //add the header in output file
	// Now one by one get the minimum element from MinElement and replace it with next element.
	// run till all filled input files reach EOF

	while (count != i)
	{
		// Get the minimum element
		// and store it in output file
		Element root = hp.getMin();

		if (isSymbolInFileName == true)
		{

			std::string line;

			std::string delim(", ");
			line = root.symbol + delim;

			line = line + root.line_;

			//cout << "Root Line " << line << endl;
			fprintf(out, "%s\n", line.c_str());
		}
		else
		{
			fprintf(out, "%s\n", root.line_.c_str());
		}

		char file_contents[1024];

		if (fscanf(in[root.i], "%[^\n] ", file_contents) != EOF)
		{

			root.line_.assign(file_contents);
			while (m_Utils->ParseContents(root.line_, root.t_, root.epoc_, root.millis_, root.symbol) == false)
			{
				if (fscanf(in[root.i], "%[^\n] ", file_contents) == EOF)
				{
					printf("Exit Loop 1 \n");
					root.epoc_ = LONG_LONG_MAX;
					count++;
					break;
				}
				root.line_.assign(file_contents);
			}
		}
		else
		{

			root.epoc_ = LONG_LONG_MAX;
			count++;
			if (count == i)
				break;
		}

		// Replace root with next
		// element of input file

		hp.replaceMin(root);
	}

	for (int i = 0; i < num_partitions; i++)
		fclose(in[i]);
	fclose(out);
}
/**
 * Start External Sort of files in ./data directory
 * @param[in] num_entries size of fileinfo vector
 * @param[in] fileinfo input filenames vector 
   
 */
int SortFiles::StartSorting(std::vector<FileInfo *> &fileinfo, long num_entries)
{
	int filenum = 0;
	std::vector<FileInfo *> tmpfiles;
	while (fileinfo.size() != 1)
	{
		//if number of files available is < than whats allowed to open ,
		//open and sort all files at one go and create one single output file

		if (num_entries <= ALLOWED_BATCH_SIZE)
		{
			std::string output_file = "output_" + std::to_string(filenum++);
			BatchExternalMergeSortFiles((char *)output_file.c_str(), fileinfo, 0, num_entries);
			FileInfo *tmpfile = new FileInfo(output_file, true);
			tmpfiles.push_back(tmpfile);
		}
		else
		{
			int N = 0;

			//Sort the files in batches = ALLOWED_BATCH_SIZE at a time
			//Should be even better if done in multiple threads with smaller batch size.

			while (((num_entries - N) >= ALLOWED_BATCH_SIZE))
			{

				std::string output_file = "output_" + std::to_string(filenum++);

				BatchExternalMergeSortFiles((char *)output_file.c_str(), fileinfo, N, N + ALLOWED_BATCH_SIZE);
				N += ALLOWED_BATCH_SIZE;
				FileInfo *tmpfile = new FileInfo(output_file, true);
				tmpfiles.push_back(tmpfile); //save the output file names
			}

			// num files left is < ALLOWED_BATCH_SIZE so whats left in FileInfo vector now sort them at one go

			if ((num_entries - N) > 0)
			{
				printf(">>>%d\n", num_entries);

				std::string output_file = "output_" + std::to_string(filenum++);
				BatchExternalMergeSortFiles((char *)output_file.c_str(), fileinfo, N, num_entries);
				FileInfo *tmpfile = new FileInfo(output_file, true);
				tmpfiles.push_back(tmpfile); //save the output file names
			}
		}
		for (int i = 0; i < fileinfo.size(); i++)
		{
			//delete the files if they are marked as tmp files
			if (fileinfo[i]->m_isTmpFile == true)
				m_Utils->DeleteFile(fileinfo[i]->m_Filename.c_str());
		}
		fileinfo.clear();
		//save the tmpfiles for next batch of sorting
		fileinfo = tmpfiles;
		num_entries = fileinfo.size();
		tmpfiles.clear();
	}
	//print the result output file
	printf("##### Final SORTED File=%s\n", fileinfo[0]->m_Filename.c_str());
	return 0;
}
