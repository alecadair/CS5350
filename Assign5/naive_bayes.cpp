/*
 * naive_bayes.cpp
 *
 *  Created on: Nov 26, 2017
 *      Author: adair
 */

#include "naive_bayes.h"
#include <iostream>
#include <sstream>
#include <fstream>

NaiveBayes::NaiveBayes() {
	// TODO Auto-generated constructor stub
	label_positive_prob = 0;
	num_features = 0;

}

NaiveBayes::~NaiveBayes() {
	// TODO Auto-generated destructor stub
}
double NaiveBayes::calculate_probability_feature(unsigned int feature, unsigned int label, double smoothing_term){
	//iterate through entire training set and find examples with label
	double positives = 0, total = 0;
	for(unsigned i = 0; i < training_vectors.size(); i++){
		//check to see if example contains feature
		map<unsigned int, double> example = training_vectors[i];

		map<unsigned int, double>::iterator map_iter = example.find(feature);
		if(map_iter != example.end()){
			positives ++;
		}
		total ++;
	}
	positives += (double)smoothing_term;
	total += num_features;
	return (positives/total);
}

void NaiveBayes::run_nbayes(string training_file, double smoothing_term){
	fill_training_vecs(training_file,0);
	//iterate through each feature
	for(int i =  1; i < num_features; i++){
		//iterate through training set and on positives add
		double pos_prob = calculate_probability_feature(i,1, smoothing_term);
		double neg_prob = 1 - pos_prob;
		Pair feature_pair;
		feature_pair.positive_prob = pos_prob;
		feature_pair.negative_prob = neg_prob;

	}
	//iterate through entire training set
	//for each example iterate through all features update probability map
	return;
}
void NaiveBayes::run_training(string filename){


}
double NaiveBayes::get_example_from_data(string line,
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
		if(file_index > num_features){
			num_features = file_index;
		}
		Pair new_pair;
		new_pair.negative_prob = 0;
		new_pair.positive_prob = 0;
		probability_map[file_index] = new_pair;
		//if(!is_test)
		//	weights[file_index] = 0;
	}
	return 0;
}

void NaiveBayes::fill_training_vecs(string filename, char is_test){
	ifstream train_stream(filename);
	string train_line;
	unsigned int lin_count = 1;
	double accuracy = 0, right = 0, wrong = 0, negs = 0, pos = 0;
	if (train_stream.is_open()) {
		while (getline(train_stream, train_line)) {
			map<unsigned int, double> feat_vec;
			double label = 0;
			get_example_from_data(train_line, &feat_vec, &label, is_test);
			if(label == -1){
				negs++;
			}else{pos++;}

			if (!is_test) {
				training_vectors.push_back(feat_vec);
				training_labels.push_back(label);
			}else{
				//double dot_prod = dot_product(feat_vec, weights);
				//double test_label = 0;
				//if (dot_prod < 0) {
				//	test_label = -1;
				//	negs++;
				//} else {
				//	test_label = 1;
				//	pos++;
				//}
				//if (test_label == label)
				//	right++;
				//else
				//	wrong++;
			}
		}
	}
	label_positive_prob = (pos/(pos+negs));
	train_stream.close();
	//if (is_test) {
	//	cout << "Accuracy\t" << ((right) / (right + wrong)) << endl;
	//	cout << "Right:\t" << right << endl;
	//	cout << "Wrong:\t" << wrong << endl;
	//	cout << "Pos:\t" << pos << endl;
	//	cout << "Neg:\t" << negs << endl;
	//}
}
