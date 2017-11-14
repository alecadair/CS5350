/*
 * file_extraction.cpp
 *
 *  Created on: Nov 13, 2017
 *      Author: adair
 */
#include "file_extraction.h"

void FileExtraction::get_example_from_line(string line, vector<double>* result, double* label){
	vector<double> feature_vector(17, 0);
	stringstream line_stream(line);
	vector<string> line_tokens;
	string label_str;
	line_stream >> label_str;
	*label = atof(label_str.c_str());
	//unsigned long index = 0;
	//for each token in line extract feature value
	result->at(result->size() -1) = 1;
	for(string token; line_stream >> token;){
		stringstream feature_stream(token);
		string ind;
		getline(feature_stream, ind, ':');
		unsigned int file_index = (unsigned int)stoi(ind);
		string feat_str;
		getline(feature_stream, feat_str, ':');
		double feat_val = (double)stof(feat_str);
		result->at(file_index - 1) = feat_val;
	}
}
