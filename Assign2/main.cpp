/*
 * main.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: alecadair
 */

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#define NUM_FEATURES 69 //includes bias

using namespace std;

const string TRAINING_FILE = "./Dataset/phishing.train";


void populate_feature_table(vector<double>* labels, vector<vector<double> >* feature_table) {
	string line_str;
	ifstream training_file_stream(TRAINING_FILE);
	if (training_file_stream.is_open()) {
		//get line file and pasre into tokens
		unsigned lines = 0;
		unsigned label_index = 0;
		while (getline(training_file_stream, line_str)) {
			//feature_vector gets pushed to feature_table at the end of loop
			vector<double> feature_vector(NUM_FEATURES, 0);
			feature_vector.at(NUM_FEATURES -1) = 1;
			stringstream line_stream(line_str);
			vector<string> line_tokens;
			//push label for example
			string label_str;
			line_stream >> label_str;
			double label_val = atof(label_str.c_str());
			labels->push_back(label_val);
			label_index ++;
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
				feature_vector.at(file_index - 1) = feat_val;
			}
			feature_table->push_back(feature_vector);

			//if (lines < 10) {
//				for (int j = 0; j < feature_table->at(lines).size(); j++) {
//					cout << feature_table->at(lines).at(j) << " ";
//				}
//				cout << labels->at(lines) << endl;
//				cout << endl;
			//}
			lines++;
		}
	}
}
double dot_product(vector<double> v1, vector<double> v2){
	double result = 0;
	for(unsigned i = 0; i < NUM_FEATURES; i++){
		result += v1.at(i) * v2.at(i);
	}
	return result;
}

vector<double> scale_vector(vector<double> v1, double scalar){
	for(unsigned i = 0; i < NUM_FEATURES; i++){
		v1.at(i) *= scalar;
	}
	return v1;
}

vector<double> vector_add(vector<double> v1, vector<double> v2){
	vector<double> result(NUM_FEATURES,0);
	for(unsigned i = 0; i < NUM_FEATURES; i++){
		result.at(i) = v1.at(i) + v2.at(i);
	}
	return result;
}

void update_weights(vector<double>* weights, vector<double> feature, double label, double r){
	double dot_prod = dot_product(*weights,feature);
	double y = 0;
	if(dot_prod <= 0)
		y = -1;
	else
		y = 1;
	if(y != label){
		vector<double> scaled_vec = scale_vector(feature,label);
		scaled_vec = scale_vector(scaled_vec,r);
		*weights = vector_add(*weights,scaled_vec);
	}
}



int main(int argc, char** argv) {
	vector<double> labels;
	vector<vector<double> > feature_table;
	vector<double> weights(NUM_FEATURES, 0);
	double rates[5] = {.001, .01, .05, .1, .25};
//	for (unsigned i = 0; i < NUM_FEATURES; i++) {
//		labels.push_back(0);
//
//	}
	populate_feature_table(&labels, &feature_table);
	//run perceptron
	for(unsigned i = 0; i < feature_table.size(); i++){
		update_weights(&weights, feature_table.at(i),labels.at(i),rates[1]);
	}

	//check accuracy of linear threshold unit
	double total_tests = 0, positives = 0, negatives = 0;
	for(unsigned i =0; i < NUM_FEATURES; i++){
		double dot_prod = dot_product(weights,feature_table.at(i));
		double test_label = 0;
		if(dot_prod <= 0)
			test_label = -1;
		else
			test_label = 1;
		if(test_label == labels[i])
			positives ++;
		else
			negatives ++;
		total_tests ++;
	}
	double accuracy = positives/total_tests;
	cout << "Accuracy:\t" << accuracy << endl;
	cout << "Inaccuray:\t" << (1-accuracy) << endl;
	return 0;
}

