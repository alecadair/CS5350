/*
 * file_extraction.h
 *
 *  Created on: Nov 13, 2017
 *      Author: adair
 */

#ifndef FILE_EXTRACTION_H_
#define FILE_EXTRACTION_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class FileExtraction{

public:
	void get_example_from_line(string line, vector<double>* result, double* label);

};

#endif /* FILE_EXTRACTION_H_ */
