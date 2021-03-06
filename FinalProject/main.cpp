/*
 * main.cpp
 *
 *  Created on: Nov 12, 2017
 *      Author: adair
 */

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>
#include "nearest_neighbors.h"
#include "id3.h"
#include "bagged_forest.h"
#include "kmeans.h"

const unsigned int NUM_FEATURES = 17;

using namespace std;

vector<double> weight_vector;

void get_example_from_data(string line, vector<double>* result, double* label){
	vector<double> feature_vector(NUM_FEATURES, 0);
	stringstream line_stream(line);
	vector<string> line_tokens;
	string label_str;
	line_stream >> label_str;
	*label = atof(label_str.c_str());
	//unsigned long index = 0;
	//for each token in line extract feature value
	for(string token; line_stream >> token;){
		stringstream feature_stream(token);
		string ind;
		getline(feature_stream, ind, ':');
		unsigned int file_index = (unsigned int)stoi(ind);
		string feat_str;
		getline(feature_stream, feat_str, ':');
		double feat_val = (double)stof(feat_str);
		result->at(file_index - 1) = feat_val;
	}
}

double dot_product(vector<double> v1, vector<double> v2){
	double result = 0;
	for(unsigned int i = 0; i<v1.size(); i++){
		result += v1.at(i)*v2.at(i);
	}
	return result;
}
bool update_weights(vector<double>* weights, vector<double> example, double label, double learning_rate){
	double dot_prod = dot_product(*weights, example);
	double y = 0;
	if(dot_prod >= 0)
		y = 1;
	else
		y = -1;
	if(y != label){
		for(unsigned int i = 0; i < weights->size(); i++){
			weights->at(i) = weights->at(i) + learning_rate*label*example[i];
		}
		return true;
	}
	return false;
}

void initialize_weights(){
	srand(time(NULL));
	for(unsigned i = 0; i < NUM_FEATURES; i++){
		double random = 0;
		//srand(time(NULL));
		int rand_num = rand();
		rand_num = rand_num % 1000;
		random = (double) rand_num;
		random = random /10000;
		if(rand_num%2 == 0){
			random *= -1;
		}
		weight_vector.push_back(random);
	}
}
void run_perceptron(){
	initialize_weights();
	double rates[3] = {.1,1,10};
	//double num_rates = 3;
	unsigned int  updates = 0;
	int lin_num = 0;
	ifstream tf_stream("./DatasetRetry/data-splits/data.train");\
	if(tf_stream.is_open()){
		double label = 0;
		string line;
		//int lin_num = 1;
		while(getline(tf_stream, line)){
			vector<double> example(NUM_FEATURES, 0);
			example[example.size() -1] = 1;
			get_example_from_data(line, &example, &label);
			if(label == 0)
				label = -1;
			if(update_weights(&weight_vector,example, label, .01)){
				updates++;
			}
			lin_num ++;
		}
	}
	tf_stream.close();
	//cout << "Training Examples:\t" << lin_num << endl;
	//cout << "Total Updates:\t" << updates << endl;

}

//void initialize_weights(){
//	srand(time(NULL));
//	for(unsigned i = 0; i < NUM_FEATURES; i++){
//		double random = 0;
//		//srand(time(NULL));
//		int rand_num = rand();
//		rand_num = rand_num % 1000;
//		random = (double) rand_num;
//		random = random /10000;
//		if(rand_num%2 == 0){
//			random *= -1;
//		}
//		weight_vector.push_back(random);
//	}
//}

void test_perceptron(){
	//cout << "Id,Prediction" << endl;
	ifstream testf_stream("./DatasetRetry/data-splits/data.test");
	ifstream testfid_stream("./DatasetRetry/data-splits/data.test.id");
	//ifstream userid_stream ("./")
	//ofstream output_data("./perceptron_data.csv");
	//ofstream output_accuracy("./perceptron_accuracy.csv");
	cout << "Id,Prediction" << endl;
	double positives = 0, negatives = 0, total_tests = 0;
	if(testf_stream.is_open() && testfid_stream.is_open()){
		double label = 0;
		string line;
		string id;
		while(getline(testf_stream, line)){
			getline(testfid_stream,id);
			vector<double> example(NUM_FEATURES,0);
			example[example.size() -1] = 1;
			get_example_from_data(line, &example, &label);
			if(label == 0)
				label = -1;
			double test_prod = dot_product(weight_vector, example);
			if(test_prod > 0)
				test_prod = 1;
			else
				test_prod = -1;
			if(test_prod == label)
				positives ++;
			else
				negatives ++;
			total_tests ++;
			//cout << id << ",";
			cout << id << ",";
			if(test_prod == -1){
				cout << '0' << endl;
			}else{
				cout << '1' << endl;
			}
		}
	}
	testf_stream.close();
	testfid_stream.close();
	double accuracy = positives/total_tests;
	cout << "Accuracy:\t" << accuracy << endl;
	cout << "Total Tests:\t" << total_tests << endl;
	cout << "Total Right:\t" << positives << endl;
	cout << "Total Wrong:\t" << negatives << endl;
}

int main(int argc, char** argv){
	//initialize_weights();
	//run_perceptron();
	//test_perceptron();
	//KNearestNeighbors knn;
	//knn.run_knn();
	//ID3 id3;
	//Node id3_node;
	//int i = 0 + 1;
	//id3_node = id3.induce_tree(10,"./DatasetRetry/data-splits/data.train");
	//id3.test("./DatasetRetry/data-splits/data.eval.anon", id3_node);
	////BaggedForest forest;
	//forest.run_bagged_forest("./DatasetRetry/data-splits/data.train");
	//forest.test("./DatasetRetry/data-splits/data.eval.anon");
	KMeans kmeans;
	kmeans.run_kmeans("./DatasetRetry/data-splits/data.train", 3, 1);
	kmeans.test_kmeans("./DatasetRetry/data-splits/data.test");
	return 0;
}

