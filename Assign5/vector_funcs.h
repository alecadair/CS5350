#ifndef VECTOR_FUNCS_H_
#define VECTOR_FUNCS_H_
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <map>

using namespace std;
class VectorFuncs{
public:
	double dot_product(map<unsigned int, double> v1, map<unsigned int, double> w); 
    void scale_vector(double s, map<unsigned int, double>* v);
	double get_example_from_data(string line, map<unsigned int, double>* feature_vector,
							double* label, char is_test/*, map<unsigned int, double>* weights*/);
};


#endif
