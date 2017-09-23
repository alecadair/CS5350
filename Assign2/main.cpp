/*
 * main.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: alecadair
 */

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#define NUM_FEATURES 69 //includes bias

using namespace std;

const string TRAINING_FILE = "./Dataset/phishing.train";

bool get_example_from_data(string line, vector<double>* result, double* label) {
	vector<double> feature_vector(NUM_FEATURES, 0);
	feature_vector.at(NUM_FEATURES - 1) = 1;
	stringstream line_stream(line);
	vector<string> line_tokens;
	//push label for example
	string label_str;
	line_stream >> label_str;
	*label = atof(label_str.c_str());
	//iterate through features e.g. "(index:value)
	unsigned index = 0;
	//stream each token to fill vectors
	for (string token; line_stream >> token;) {
		stringstream feature_stream(token);
		string ind;
		getline(feature_stream, ind, ':');
		unsigned file_index = (unsigned) stoi(ind);
		string feat_str;
		getline(feature_stream, feat_str, ':');
		double feat_val = (double) stof(feat_str);
		result->at(file_index - 1) = feat_val;
	}
	return true;
}

double dot_product(vector<double> v1, vector<double> v2) {
	double result = 0;
	for (unsigned i = 0; i < NUM_FEATURES; i++) {
		result += v1.at(i) * v2.at(i);
	}
	return result;
}

vector<double> scale_vector(vector<double> v1, double scalar) {
	for (unsigned i = 0; i < NUM_FEATURES; i++) {
		v1.at(i) *= scalar;
	}
	return v1;
}

vector<double> vector_add(vector<double> v1, vector<double> v2) {
	vector<double> result(NUM_FEATURES, 0);
	for (unsigned i = 0; i < NUM_FEATURES; i++) {
		result.at(i) = v1.at(i) + v2.at(i);
	}
	return result;
}

void update_weights(vector<double>* weights, vector<double> feature,
		double label, double r) {
	double dot_prod = dot_product(*weights, feature);
	double y = 0;
	if (dot_prod < 0)
		y = -1;
	else
		y = 1;
	if (y != label) {
		vector<double> scaled_vec = scale_vector(feature, label);
		scaled_vec = scale_vector(scaled_vec, r);
		*weights = vector_add(*weights, scaled_vec);
	}
}

double perceptron(double* total, double* pos, double* neg,
		string training_files[4], string test_file, double learning_rate) {
	vector<double> weights(NUM_FEATURES, 0);
	srand(time(NULL));
	//fill weights up
	for (unsigned i = 0; i < NUM_FEATURES; i++) {
		double random = 0;
		int rand_num = rand();
		rand_num = rand_num % 1000;
		unsigned char mod = rand_num %2;
		random = (double) rand_num;
		//random =(double) rand_num%(1000);
		random = (random / (10000));
		if(mod)
			random *= -1;
		weights.at(i) = random;
	}
	//run perceptron
	for (unsigned i = 0; i < 4; i++) {
		ifstream tf_stream(training_files[i]);
		if (tf_stream.is_open()) {
			vector<double> example(NUM_FEATURES, 0);
			double label = 0;
			string line;
			while (getline(tf_stream, line)) {
				get_example_from_data(line, &example, &label);
				//while(get_example_from_data(tf_stream, &example, &label)){
				//for(unsigned i = 0; i < feature_table.size(); i++){
				update_weights(&weights, example, label, learning_rate);
			}
		}
	}
	//check accuracy of linear threshold against test file
	ifstream test_stream(test_file.c_str());
	double total_tests = 0, positives = 0, negatives = 0;
	string test_line;
	if (test_stream.is_open()) {
		while (getline(test_stream, test_line)) {
			vector<double> test_example(NUM_FEATURES, 0);
			double examp_label = 0;
			get_example_from_data(test_line, &test_example, &examp_label);
			double dot_prod = dot_product(weights, test_example);
			//double dot_prod = dot_product(weights,test_features[i].at(j));
			double test_label = 0;
			if (dot_prod < 0)
				test_label = -1;
			else
				test_label = 1;
			if (test_label == examp_label)
				positives++;
			else
				negatives++;
			total_tests++;
		}
		test_stream.close();
	}
	double accuracy = positives / total_tests;
	*total = total_tests;
	*pos = positives;
	*neg = negatives;
	return accuracy;
	//cout << "Accuracy:\t" << accuracy << endl;
	//cout << "Inaccuray:\t" << (1-accuracy) << endl;
}

void five_fold_cross_validation() {
	double rates[3] = { 1, .1, .01 };
}

int main(int argc, char** argv) {
	double rates[3] = { 1, .1, .01 };
	//run 5-fold cross validation on perceptron
	string file_names[5] = { "./Dataset/CVSplits/training00.data",
			"./Dataset/CVSplits/training01.data",
			"./Dataset/CVSplits/training02.data",
			"./Dataset/CVSplits/training03.data",
			"./Dataset/CVSplits/training04.data" };

	for (unsigned rate_index = 0; rate_index < 3; rate_index++) {
		for (unsigned i = 0; i < 5; i++) {
			string test_file = file_names[i];
			vector<string> training_files;
			string training_file_array[4];
			//fill test files vector for perceptron
			for (unsigned j = 0; j < 5; j++) {
				if (j != i) {
					training_files.push_back(file_names[j]);
				}
			}

			for (unsigned j = 0; j < 4; j++) {
				training_file_array[j] = training_files[j];
			}
			double total = 0, pos = 0, neg = 0;
			double learning_rate = rates[rate_index];
			double accuracy = perceptron(&total, &pos, &neg,
					training_file_array, test_file, learning_rate);
			cout << "Learning Rate:\t" << learning_rate << endl;
			cout << "Test File: " << "training0" << i << ".data" << endl;
			cout << "Accuracy:\t" << accuracy << "\t" << pos << "/" << total
					<< endl;
			cout << "Inaccuracy:\t" << (1 - accuracy) << endl;
			cout << endl;
		}
	}
	return 0;
}

