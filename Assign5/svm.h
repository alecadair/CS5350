/*
 * svm.h
 *
 *  Created on: Nov 24, 2017
 *      Author: adair
 */

#ifndef SVM_H_
#define SVM_H_
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class SVM {
public:
	//map<unsigned int, double> feature_vector;
	vector<map<unsigned int, double> > training_vectors;
	vector<double> training_labels;
	vector<double> learning_rates = {10,1,.1,.001,.0001,.00001};
	vector<double> trade_off_param = {10,1,.1,.001,.0001,.00001};
	map<unsigned int, double> weights;
	SVM();
	virtual ~SVM();
	void run_svm(double learning_rate, double trade_off);
	void initialize_weights();
	void fill_training_vecs(string filename, char is_test);
	void shuffle_data();
	//returns the label in double form for the given string
	double get_example_from_data(string line, map<unsigned int, double>* feature_vector,double* label);
	double dot_product(map<unsigned int, double> v1, map<unsigned int, double> w);
	void scale_vector(double s, map<unsigned int, double>* v);
	void test_accuracy(string filename);
};

#endif /* SVM_H_ */
