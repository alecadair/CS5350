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

#define NUM_FEATURES 70 //includes bias

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
	for (unsigned i = 0; i < v1.size(); i++) {
		result = result + v1.at(i) * v2.at(i);
	}
	return result;
}

double find_max(vector<double> v1, unsigned* max_index){
	double max = 0;
	for(unsigned i = 0; i < v1.size(); i++){
		if(max <= v1.at(i)){
			max = v1.at(i);
			*max_index = i;
		}
	}
	return max;
}

vector<double> scale_vector(vector<double> v1, double scalar) {
	for (unsigned i = 0; i < v1.size(); i++) {
		v1.at(i) *= scalar;
	}
	return v1;
}

vector<double> vector_add(vector<double> v1, vector<double> v2) {
	vector<double> result(v1.size(), 0);
	for (unsigned i = 0; i < v1.size(); i++) {
		result.at(i) = v1.at(i) + v2.at(i);
	}
	return result;
}

vector<double> initialize_weights(){
	vector<double> weights(NUM_FEATURES,0);
	//vector<double> weights(NUM_FEATURES, 0);
	srand(time(NULL));
	//fill weights up
	for (unsigned i = 0; i < NUM_FEATURES; i++) {
		double random = 0;
		int rand_num = rand();
		rand_num = rand_num % 1000;
		unsigned char mod = rand_num %2;
		random = (double) rand_num;
		random = (random / (1000000));
		if(mod)
			random *= -1;
		weights.at(i) = random;
	}
	return weights;
}

bool update_weights(vector<double>* weights, vector<double> feature,
		double label, double r) {
	double dot_prod = dot_product(*weights, feature);
	double num_updates = 0;
	double y = 0;
	if (dot_prod <= 0)
		y = -1;
	else
		y = 1;
	if (y != label) {
		for(unsigned i = 0; i < weights->size(); i++){
			weights->at(i) = weights->at(i) + r*label*feature[i];
		}
		return true;
	}
	return false;

}
double test_file_against_weights(double* total, double* pos, double* neg, vector<double>* weights, string test_file){
	ifstream test_stream(test_file.c_str());
		double total_tests = 0, positives = 0, negatives = 0;
		string test_line;
		if (test_stream.is_open()) {
			while (getline(test_stream, test_line)) {
				vector<double> test_example(NUM_FEATURES, 0);
				double examp_label = 0;
				get_example_from_data(test_line, &test_example, &examp_label);
			//	test_example[NUM_FEATURES -1] = 1;
				double dot_prod = dot_product(*weights, test_example);
				double test_label = 0;
				if (dot_prod <= 0)
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
}

void perceptron(vector<double>* weights,vector<string> training_files,
								double learning_rate, double* num_updates) {
	//run perceptron
	for (unsigned i = 0; i < training_files.size(); i++) {
		ifstream tf_stream(training_files[i]);
		*num_updates = 0;
		if (tf_stream.is_open()) {
//			vector<double> example(NUM_FEATURES, 0);
			double label = 0;
			string line;
			int line_num = 1;
			while (getline(tf_stream, line)) {
				vector<double> example(NUM_FEATURES, 0);
				example[example.size() -1] = 1;
				get_example_from_data(line, &example, &label);
				if(update_weights(weights, example, label, learning_rate))
					*num_updates += 1;
				line_num ++;
			}
		}
		//cout << "\tNumber of Updates for file: " << training_files[i] << "\t" << *num_updates << endl;
	}
	//cout << endl;
}

void five_fold_cross_validation(double learning_rate, double* num_updates, vector<double>* weights) {
	//unsigned NUM_RATES = 3;
	//double rates[3] = { 1, .1, .01 };
	//run 5-fold cross validation on perceptron
	vector<string> file_names = { "./Dataset/CVSplits/training00.data",
			"./Dataset/CVSplits/training01.data",
			"./Dataset/CVSplits/training02.data",
			"./Dataset/CVSplits/training03.data",
			"./Dataset/CVSplits/training04.data" };
	//vector<string> file_names = {"./Dataset/phishing.train", "./Dataset/phishing.train"};
	double mean_accuracy = 0;
	//for (unsigned rate_index = 0; rate_index < NUM_RATES; rate_index++) {
	for (unsigned i = 0; i < file_names.size(); i++) {
		string test_file = file_names[i];
		vector<string> training_files;

		//fill test files vector for perceptron
		for (unsigned j = 0; j < file_names.size(); j++) {
			if (j != i) {
				training_files.push_back(file_names[j]);
			}
		}
		double total = 0, pos = 0, neg = 0;
		//double learning_rate = rates[rate_index];
		perceptron(weights, training_files, learning_rate, num_updates);
		//double accuracy = test_file_against_weights(&total, &pos, &neg, weights,test_file);
		//cout << "Learning Rate:\t" << learning_rate << endl;
		//cout << "Test File: " << training_files[i] << endl;
		//cout << "Accuracy:\t" << accuracy << "\t" << pos << "/" << total
		//<< endl;
		//cout << "Inaccuracy:\t" << (1 - accuracy) << endl;
		//cout << endl;
		//mean_accuracy += accuracy;
	}
	//cout << "Mean Accuracy:\t" << mean_accuracy / (file_names.size()) << endl
	//		<< endl;
	//return (mean_accuracy / file_names.size());
	//}
}

double test_perceptron(string test_file, double r, double* num_updates){
	double accuracy = 0;
	string training_file = "./Dataset/phishing.train";
	double total = 0, pos = 0, neg = 0;
	vector<string> training_file_vec;
	training_file_vec.push_back(training_file);
	//accuracy = perceptron(&total, &pos, &neg, training_file_vec,test_file, r, num_updates);

	return accuracy;
}

void run_test_suite(){
	//run simple perceptron
	double rates[3] = {1,.1,.01};
	double num_rates = 3;
	double num_updates = 0;

	cout << "Running and Training with Simple Perceptron" << endl;
	vector<double> accuracies(3, 0);
	//vector<vector<double> > weights_table
	cout << "Running cross validation for ten epochs for each hyper-parameter." << endl;
	for(unsigned i = 0; i < num_rates; i ++){
		vector<double> weights = initialize_weights();
		double curr_accuracy = 0;
		for(unsigned j = 0; j < 10; j++)
			/*curr_accuracy =*/ five_fold_cross_validation(rates[i], &num_updates,&weights);
		double total = 0, pos = 0, neg = 0;
		curr_accuracy = test_file_against_weights(&total,&pos,&neg,&weights,"./Dataset/phishing.dev");
		accuracies.at(i) = (curr_accuracy);
		cout <<"\tAccuracy for rate " << rates[i] <<": "<<accuracies[i]  << endl;
	}

	double max_accuracy = 0;
	unsigned accuracy_index = 0;
	for(unsigned i = 0; i < accuracies.size(); i++){
		if(accuracies[i] >= max_accuracy){
			max_accuracy = accuracies[i];
			accuracy_index = i;
		}
	}
	//cout << "Simple Perceptron" << endl;
	cout << "Max Accuracy:\t" << max_accuracy << endl;
	double max_rate = rates[accuracy_index];
	cout << "Best learning rate:\t" << max_rate << endl;
	num_updates = 0;
	double simple_accuracy = test_perceptron("./Dataset/phishing.dev", max_rate,&num_updates);
	//cout << "Number of updates for "
	//run training for 20 epochs with best learning rat>
	vector<double> weights = initialize_weights();
	num_updates = 0;
	vector<string> training_file_vec;
	vector<vector<double> > weight_vectors;
	vector<double> training_accuracies;
	training_file_vec.push_back("./Dataset/phishing.train");
	cout << "Training with Learning rate " << max_rate << " on File Dataset/phishing.train" << endl;
	for(unsigned i = 0; i < 20; i++){
		double curr_accuracy = 0, total = 0, pos = 0, neg = 0;
		perceptron(&weights,training_file_vec,max_rate,&num_updates);
		curr_accuracy = test_file_against_weights(&total, &pos, &neg, &weights, "./Dataset/phishing.dev");
		training_accuracies.push_back(curr_accuracy);
		weight_vectors.push_back(weights);
		cout << "Epoch " << i <<" accuracy - " << curr_accuracy << endl;
	}
	unsigned max_index = 0;
	double highest_acc = find_max(training_accuracies,&max_index);
	cout << "Highest accuracy at Epoch " << max_index << " with accuracy " << highest_acc << endl;
	vector<double> best_classifier = weight_vectors.at(max_index);
	double test_accuracy = 0, total = 0, pos = 0, neg = 0;
	test_accuracy = test_file_against_weights(&total, &pos, &neg, &best_classifier, "./Dataset/phishing.test");
	cout << "Accuracy for phishing.test " <<test_accuracy <<endl;
}

int main(int argc, char** argv) {
	run_test_suite();
	return 0;
}

