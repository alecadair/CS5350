/*
 * kmeans.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: alec
 */

#include "kmeans.h"
#include <fstream>
#include <ostream>
#include <math.h>
#include <sstream>
#include <limits>

KMeans::KMeans() {
	// TODO Auto-generated constructor stub

}

KMeans::~KMeans() {
	// TODO Auto-generated destructor stub
}

void KMeans::run_kmeans(string filename, int K, int num_iterations){
	initialize_means(filename,K);
	ifstream training_stream(filename);
	char no_change = 1;
	for(unsigned i = 0; i < num_iterations; i++){
		for(unsigned j = 0; j < training_data.size(); j++){
			double min_distance = std::numeric_limits<double>::max();
			unsigned min_index = 0;
			for(unsigned k = 0; k < means.size(); k++){
				double distance = calculate_distance(training_data[j].data, means[k].data);
				if(distance <= min_distance){
					min_distance = distance;
					min_index = k;
				}
			}
			training_data[j].cluster = min_index;
			means[min_index].points_in_cluster ++;
			for(unsigned k = 0; k < training_data[0].data.size(); k++){
				double jnew_mean = means[min_index].data[k];
				double n = means[min_index].points_in_cluster;
				double feature_val = training_data[i].data[k];
				jnew_mean = ((jnew_mean*(n-1))+ feature_val)/(float)n;

				means[min_index].data[k] = jnew_mean;
				//means[min_index].data[j] = (means[min_index].data[j]);
			}
			if(training_data[j].cluster != min_index){
				no_change = 0;
			}
			training_data[j].cluster = min_index;
		}

	}
	classify_cluster();
}

void KMeans::classify_cluster(){
	double c0pos = 0, c0neg = 0;
	double c1pos = 0, c1neg = 0;
	double c2pos = 0, c2neg = 0;
	for(unsigned int i = 0; i < training_data.size(); i++){
		if(training_data[i].cluster == 0){
			if(training_data[i].label == 1){
				c0pos ++;
			}else{
				c0neg ++;
			}
		}else if(training_data[i].cluster == 1){
			if(training_data[i].label == 1){
				c1pos++;
			}else{
				c1neg ++;
			}
		}else{
			if(training_data[i].label ==1){
				c2pos ++;
			}else{
				c2neg++;
			}
		}
	}
	if(c0pos > c0neg)
		means[0].label = 1;
	else
		means[0].label = 0;
	if(c1pos > c1neg)
		means[1].label = 1;
	else
		means[1].label = 0;
	if(c2pos > c2neg)
		means[2].label = 1;
	else
		means[2].label = 0;

}

void KMeans::test_kmeans(string filename){
	ifstream test_stream(filename);
	string test_line;
	double right = 0, wrong = 0, pos = 0, neg = 0;
	while(getline(test_stream,test_line)){
		vector<double> example;
		double test_label = 0;
		get_example_from_line(test_line,&example,&test_label);
		double min_distance = std::numeric_limits<double>::max();
		unsigned min_index = 0;
		for(unsigned k = 0; k < means.size(); k++){
			double distance = calculate_distance(example, means[k].data);
			if(distance <= min_distance){
				min_distance = distance;
				min_index = k;
			}
		}
		if(test_label == means[min_index].label){
			right++;
		}else{
			wrong ++;
		}
		if(means[min_index].label == 1)
			pos++;
		else
			neg++;
	}
	cout << "Accuracy: " << ((right)/(right+wrong)) << endl;
	cout << "Positives: " << (pos) << endl;
	cout << "Negatives: " << (neg) << endl;
}

int KMeans:: classify_test_example(Point example){
	int result = 0;
	double min_distance = std::numeric_limits<double>::max();
	unsigned min_index = 0;
	for(unsigned k = 0; k < means.size(); k++){
		double distance = calculate_distance(example.data, means[k].data);
		if(distance <= min_distance){
			min_distance = distance;
			min_index = k;
		}
	}
	return min_index;
}

//initialize means use Forgy method to choose different means
void KMeans:: initialize_means(string filename, int K){
	//unsigned int k = 3;
	ifstream training_stream(filename);
	string line;
	while(getline(training_stream,line)){
		vector<double> training_example;
		double training_label;
		get_example_from_line(line,&training_example, &training_label);
		Point p;
		p.data = training_example;
		p.label = training_label;
		p.cluster = -1;
		p.points_in_cluster = 1;
		training_data.push_back(p);
		//training_labels.push_back(training_label);
	}
	srand(time(NULL));
	for(unsigned int i = 0; i < K; i++){
		int random =rand() %(training_data.size());
		means.push_back(training_data[random]);
	}
	for(unsigned int i = 0; i < training_data.size(); i++){
		double min_distance = std::numeric_limits<double>::max();
		unsigned min_index = 0;
		for(unsigned j = 0; j < means.size(); j++){
			double distance = calculate_distance(training_data[i].data, means[j].data);
			if(distance <= min_distance){
				min_distance = distance;
				min_index = j;
			}
		}
		//means[min_index].points_in_cluster ++;
		training_data[i].cluster = min_index;
	}
}
void KMeans::get_example_from_line(string line, vector<double>* result, double* label){
	vector<double> feature_vector(17, 0);
	*result = feature_vector;
	stringstream line_stream(line);
	vector<string> line_tokens;
	string label_str;
	line_stream >> label_str;
	*label = atof(label_str.c_str());
	//unsigned long index = 0;
	//for each token in line extract feature value
	result->at(result->size() -1) = 1;
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

double KMeans::calculate_distance(vector<double> v1, vector<double> v2){
	double distance = 0;
	for(unsigned int i = 0; i < v1.size(); i++){
		distance += ((v2[i] - v1[i])*(v2[i] - v1[i]));
	}
	distance = sqrt(distance);
	return distance;
}
