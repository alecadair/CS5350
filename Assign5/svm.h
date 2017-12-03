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
#include <string>

using namespace std;

class SVM {
public:
	//map<unsigned int, double> feature_vector;

	SVM();
	virtual ~SVM();
	void run_svm(double learning_rate, double trade_off);
	void initialize_weights();
	double fill_training_vecs(string filename, char is_test);
	void shuffle_data();
	//returns the label in double form for the given string
	double get_example_from_data(string line, map<unsigned int, double>* feature_vector,double* label, char is_test);
	double dot_product(map<unsigned int, double> v1, map<unsigned int, double> w);
	void scale_vector(double s, map<unsigned int, double>* v);
	double test_accuracy(string filename);
	void cross_validation();
	double validate(double learning_rate, double tradeoff);
	void empty_vectors();
	void empty_weights();

private:
	vector<map<unsigned int, double> > training_vectors;
	vector<double> training_labels;
	vector<double> learning_rates = {10,1,.1,.001,.0001,.00001};
	vector<double> trade_off_param = {10,1,.1,.001,.0001,.00001};
	vector<string> file_list = {"./data/CVSplits/training00.data",
								"./data/CVSplits/training01.data",
								"./data/CVSplits/training02.data",
								"./data/CVSplits/training03.data",
								"./data/CVSplits/training04.data"};
	map<unsigned int, double> weights;
};

#endif /* SVM_H_ */
