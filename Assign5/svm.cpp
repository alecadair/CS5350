/*
 * svm.cpp
 *
 *  Created on: Nov 24, 2017
 *      Author: adair
 */

#include "svm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <algorithm>
#include <random>

SVM::SVM() {
	// TODO Auto-generated constructor stub

}

SVM::~SVM() {
	// TODO Auto-generated destructor stub
}

void SVM::run_svm(double learning_rate, double trade_off) {
	//shuffle_data();
	for (unsigned int i = 0; i < training_labels.size(); i++) {
		map<unsigned int, double> example;
		example = training_vectors[i];
		double dot_prod = dot_product(example, weights);
		dot_prod *= training_labels[i];
		if (dot_prod <= 1) {
			scale_vector(trade_off, &example);
			scale_vector(training_labels[i], &example);
			scale_vector(learning_rate, &example);
			scale_vector((1 - learning_rate), &weights);
			map<unsigned int, double>::iterator weights_iter = weights.begin();
			int weights_size = weights.size();
			//iterate through weights and add example
			while (weights_iter != weights.end()) {
				unsigned int ind = weights_iter->first;
				//check if example contains attribute
				auto search = example.find(ind);
				if (search != example.end()) {
					weights_iter->second += search->second;
					//cout << "weights adjust" << endl;
				}
				weights_iter++;
			}
		} else {
			scale_vector((1 - learning_rate), &weights);
		}
	}
	return;
}

void SVM:: empty_vectors(){
	for(int i = training_vectors.size() -1; i >= 0; i--){
		training_vectors.erase(training_vectors.begin() + i);
		training_labels.erase(training_labels.begin() + i);
	}
}
double SVM::dot_product(map<unsigned int, double> v1,
		map<unsigned int, double> w) {
	map<unsigned int, double>::iterator map_iter = v1.begin();
	double result = 0;
	while (map_iter != v1.end()) {
		unsigned int index = map_iter->first;
		double attr = map_iter->second;
		auto iter = w.find(index);
		if(iter != w.end()){
			double weight = w[index];
			result += (attr * weight);
		}
		map_iter++;
	}
	return result;
}
void SVM::scale_vector(double s, map<unsigned int, double>* v) {
	map<unsigned int, double>::iterator map_iter = v->begin();
	while (map_iter != v->end()) {
		map_iter->second *= s;
		map_iter++;
	}
}

double SVM::get_example_from_data(string line,
		map<unsigned int, double>* feat_vec, double* label, char is_test) {
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
		if(!is_test)
			weights[file_index] = 0;
	}
}

double SVM::fill_training_vecs(string filename, char is_test) {
	ifstream train_stream(filename);
	string train_line;
	unsigned int lin_count = 1;
	double accuracy = 0, right = 0, wrong = 0, negs = 0, pos = 0;
	if (train_stream.is_open()) {
		while (getline(train_stream, train_line)) {
			map<unsigned int, double> feat_vec;
			double label = 0;
			get_example_from_data(train_line, &feat_vec, &label, is_test);
			if (!is_test) {
				training_vectors.push_back(feat_vec);
				training_labels.push_back(label);
			}else{
				double dot_prod = dot_product(feat_vec, weights);
				double test_label = 0;
				if (dot_prod < 0) {
					test_label = -1;
					negs++;
				} else {
					test_label = 1;
					pos++;
				}
				if (test_label == label)
					right++;
				else
					wrong++;
			}
		}
	}
	train_stream.close();
	if (is_test) {
		//cout << "Accuracy\t" << ((right) / (right + wrong)) << endl;
		//cout << "Right:\t" << right << endl;
		//cout << "Wrong:\t" << wrong << endl;
		//cout << "Pos:\t" << pos << endl;
		//cout << "Neg:\t" << negs << endl;
	}
	if(right == 0 || wrong == 0)
		return 0;
	else
		return ((right) /(right+wrong));

}

void SVM::shuffle_data() {
	for (unsigned int i = training_labels.size() - 1; i > 0; i--) {
		srand((unsigned) time(0));
		int random_int = rand();
		random_int = random_int % (i + 1);
		double temp_label = training_labels[random_int];
		map<unsigned int, double> temp_map = training_vectors[random_int];
		training_labels[random_int] = training_labels[i];
		training_vectors[random_int] = training_vectors[i];
		training_labels[i] = temp_label;
		training_vectors[i] = temp_map;
	}
}

void SVM:: cross_validation(){
	for(unsigned int i = 0; i < learning_rates.size(); i++){
		for(unsigned int j = 0; j < trade_off_param.size(); j++){
			cout << "SVM Cross Validation - tradeoff " << trade_off_param[j] <<
						" learning rate " << learning_rates[i] << endl;
			double result = validate(learning_rates[i], trade_off_param[j]);
			cout << "Average accuracy " << result << endl;
		}
	}
}
double SVM:: validate(double learning_rate, double tradeoff){
	double accuracy = 0;
	for(unsigned int i = 0; i < 5; i++){
		empty_vectors();
		for(unsigned int j = 0; j < file_list.size(); j++){
			if(j != i)
				fill_training_vecs(file_list[j],0);
		}
		//fill_training_vecs(file_list[i], 0);
		run_svm(learning_rate,tradeoff);
		double temp_acc = test_accuracy(file_list[i]);
		cout << "Testing on file " << file_list[i] << endl;
		cout << "Accuracy " << temp_acc << endl;;
		accuracy += temp_acc;
		//double fill_training_vecs(string filename, char is_test);
	}
	accuracy = accuracy / 5;
	return accuracy;
}
double SVM::test_accuracy(string filename) {
	return fill_training_vecs(filename, 1);
}

void SVM:: empty_weights(){

}
