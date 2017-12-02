
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <map>
#include <vector>
#include "vector_funcs.h"
using namespace std;

double VectorFuncs::dot_product(map<unsigned int, double> v1, map<unsigned int, double> w) {
	map<unsigned int, double>::iterator map_iter = v1.begin();
	double result = 0;
	while (map_iter != v1.end()) {
		unsigned int index = map_iter->first;
		double attr = map_iter->second;
		auto iter = w.find(index);
		if(iter != w.end()){
			double weight = iter->second;
			result += (attr * weight);
		}
		map_iter++;
	}
	//cout << result << endl;
	return result;
}

void VectorFuncs::scale_vector(double s, map<unsigned int, double>* v) {
	map<unsigned int, double>::iterator map_iter = v->begin();
	while (map_iter != v->end()) {
		map_iter->second *= s;
		map_iter++;
	}
}


double VectorFuncs::get_example_from_data(string line,map<unsigned int, double>* feat_vec,
						double* label, char is_test/*,map<unsigned int, double> * weights*/){
	stringstream lin_stream(line);
	vector<string> tokens;
	string label_str;
	lin_stream >> label_str;
	*label = atof(label_str.c_str());
	unsigned int index = 0;
	for (string token; lin_stream >> token;) {
		stringstream feature_stream(token);
		string ind;
		getline(feature_stream, ind, ':');
		unsigned int file_index = (unsigned int) stoi(ind);
		string feat_str;
		getline(feature_stream, feat_str, ':');
		double feat_val = (double) stof(feat_str);
		(*feat_vec)[file_index] = feat_val;
//		if(!is_test)
//			(*weights)[file_index] = 0;
	}
}




