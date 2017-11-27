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
double NaiveBayes::calculate_probability_feature(unsigned int feature, double label,
												double smoothing_term, int limit){
	//iterate through entire training set and find examples with label
	double positives = 0, total = 0;
	for(unsigned i = 0; i < training_vectors.size(); i++){
		//check to see if example contains feature
		map<unsigned int, double> example = training_vectors[i];

		map<unsigned int, double>::iterator map_iter = example.find(feature);
		if(map_iter != example.end()){
			if(training_labels[i] == label)
				positives ++;
		}
		total ++;
		if(i >= limit)
			break;
	}
	positives += (double)smoothing_term;
	total += num_features;
	return (positives/total);
}

void NaiveBayes::run_nbayes(string training_file, double smoothing_term, int limit){
	fill_training_vecs(training_file,0);
	//iterate through each feature
	cout << "Training on " << training_file << endl;
	for(unsigned int i =  1; i < num_features; i++){
		//if(i% 25 == 0)
			cout << i+1 << " ";
		//iterate through training set and on positives add
		double poslabel_prob = calculate_probability_feature(i,1, smoothing_term, limit);
		double neg_prob = 1 - poslabel_prob;
		double neglabel_prob = calculate_probability_feature(i,-1,smoothing_term, limit);
		Pair feature_pair;
		feature_pair.positive_prob = poslabel_prob;
		feature_pair.negative_prob = neglabel_prob;
		probability_map[i] = feature_pair;
	}
	cout << endl;
	return;
}

void NaiveBayes::run_test(string filename, int limit){
	ifstream test_stream(filename);
	string test_line;
	int linum = 0;
	double pos_pred = 0;
	double right = 0, wrong = 0, tests = 0;
	cout << "Testing " << filename << endl;
	if(test_stream.is_open()){
		while(getline(test_stream, test_line)){
			if(linum % 100 == 0)
				cout << linum << " ";
			map<unsigned int, double> test_ex;
			double test_label = 0;
			get_example_from_data(test_line,&test_ex, &test_label,1);
			double pos_prob = label_positive_prob;
			double neg_prob = 1 - label_positive_prob;
			//iterate through all features and do products
			for(unsigned int i = 1; i < num_features; i ++){
				map<unsigned int, Pair>::iterator map_iter;
				map_iter = probability_map.find(i);
				if(map_iter != probability_map.end()){
					Pair probs = map_iter->second;
					pos_prob *= probs.positive_prob;
					neg_prob *= probs.negative_prob;
				}

			}
			double prediction = 0;
			if(pos_prob >= neg_prob)
				prediction = 1;
			else
				prediction = -1;
			if(prediction == 1)
				pos_pred ++;
			if(test_label == prediction)
				right ++;
			else
				wrong++;
			linum++;
			if(linum >= limit)
				break;
			tests++;
		}
	}
	test_stream.close();
	cout << endl<< "Accuracy:\t" << ((right)/(right+wrong)) << endl;
	cout << "Right:\t" << right << endl;
	cout << "Positive Predictions: " << pos_pred ++ << endl;
	cout << "Number of tests: " << tests << endl;
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
		if(file_index > num_features && !is_test){
			num_features = file_index;
		}
		if(!is_test){
			Pair new_pair;
			new_pair.negative_prob = 0;
			new_pair.positive_prob = 0;
			probability_map[file_index] = new_pair;
		}
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
			}else{
				pos++;
			}
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
