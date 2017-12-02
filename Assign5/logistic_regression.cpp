/*
 * logistic_regression.cpp
 *
 *  Created on: Nov 28, 2017
 *      Author: adair
 */
#include <stdlib.h>
#include "logistic_regression.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <cmath>
#include "svm.h"
#include "naive_bayes.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

#define TEST 0
#define NOT_TEST 1

LogisticRegression::LogisticRegression() {
	// TODO Auto-generated constructor stub
	

}

LogisticRegression::~LogisticRegression() {
	// TODO Auto-generated destructor stub
}

void LogisticRegression::test(string filename){
	ifstream test(filename);
	string line;
	unsigned int lin_count = 1;
	double accuracy = 0, right = 0, wrong = 0, negs = 0, pos = 0;
	if(test.is_open()){
		while(getline(test,line)){
			map<unsigned int, double> test_vec;
			double label = 0;
			funcs.get_example_from_data(line,&test_vec,&label,1);
			double dot_prod = funcs.dot_product(test_vec,weights);
			cout << label << " " << dot_prod << endl;
			double test_label = 0;
			if(dot_prod < 0){
				test_label = -1;
				negs++;
			}else{
				test_label = 1;
				pos++;
			}
			if(test_label == label)
				right++;
			else
				wrong++;
		}
	}
	test.close();
	cout <<"Accuracy:\t"<<((right)/(right+wrong)) << endl;
	cout <<"Right:\t" << right << endl;
	cout <<"Wrong:\t" << wrong << endl;
	cout <<"Pos:\t" << pos << endl;
	cout <<"Neg:\t" << negs << endl;
}

void LogisticRegression::train_classifier(string filename){
	ifstream training_stream(filename);
	string line;
	double gamma = learning_rate;
	double t = 0;
	if(training_stream.is_open()){
		while(getline(training_stream,line)){
			double denom = 1 + ((pow(learning_rate,t)/sqrt(tradeoff)));
			double gamma_t = learning_rate/denom; 
			map<unsigned int, double> example;
			double label = 0;
			funcs.get_example_from_data(line,&example, &label,0);
			
			update_weights(example,label,gamma_t,tradeoff);
			t++;
		}
	
	}
}

map<unsigned int, double> LogisticRegression::add_vectors(map<unsigned int, double> v1, map<unsigned int, double> v2){
	map<unsigned int, double> result;
	for(map<unsigned int, double>::iterator it = v1.begin(); it != v1.end(); it++){
		unsigned int index = it->first;
		double val = it->second;
		map<unsigned int, double>::iterator elem = v2.find(index);
		if(elem != v2.end()){
			result[index] = val + elem->second; 
		}else{
			result[index] = val;
		}
	}
	for(map<unsigned int, double>::iterator it = v2.begin(); it != v2.end(); it++){
		unsigned int index = it->first;
		double val = it->second;
		map<unsigned int, double>::iterator finder = v1.find(index);
		if(finder != v1.end()){
			result[index] = val + finder->second;
		}else{
			result[index] = val;
		}
	}
	return result;
}

void LogisticRegression::update_weights(map<unsigned int, double> feature, double label, double gamma_t, double sigma){
	//	SVM funcs2;
		VectorFuncs funcs2;
		//double* weight = &weights_it->second;
		double deriv = gamma_t;//*();
		double denom = 1 + (exp(label*(funcs2.dot_product(feature,weights))));

		funcs2.scale_vector(label*-1, &feature);
		funcs2.scale_vector((1/denom), &feature);
		map<unsigned int, double> sub_weights = weights;
		funcs2.scale_vector(2,&sub_weights);
		funcs2.scale_vector(tradeoff,&sub_weights);
		map<unsigned int, double> add_result = add_vectors(sub_weights,feature);
		funcs2.scale_vector(-1*gamma_t, &add_result);
		weights = add_vectors(weights,add_result);		
	
}








