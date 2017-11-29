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
#include "svm.h"
#include "naive_bayes.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>


LogisticRegression::LogisticRegression() {
	// TODO Auto-generated constructor stub
	

}

LogisticRegression::~LogisticRegression() {
	// TODO Auto-generated destructor stub
}

void LogisticRegression::test(string filename){
	SVM testfuncs;
	ifstream test(filename);
	string line;
	unsigned int lin_count = 1;
	double accuracy = 0, right = 0, wrong = 0, negs = 0, pos = 0;
	if(test.is_open()){
		while(getline(test,line)){
			map<unsigned int, double> test_vec;
			double label = 0;
			testfuncs.get_example_from_data(line,&test_vec,&label,1);
			double dot_prod = testfuncs.dot_product(test_vec,weights);
			cout << dot_prod << endl;
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
	if(training_stream.is_open()){
		while(getline(training_stream,line)){

			map<unsigned int, double> example;
			double label = 0;
			funcs.get_example_from_data(line,&example, &label,1);
			update_weights(example,label,learning_rate,tradeoff);

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
		}
	}
	return result;
}

void LogisticRegression::update_weights(map<unsigned int, double> feature, double label, double gamma, double sigma){
	SVM funcs2;
		//double* weight = &weights_it->second;
		double deriv = learning_rate;//*();
		double denom = 1 + (exp(label*(funcs2.dot_product(feature,weights))));
		funcs2.scale_vector(label, &feature);
		funcs2.scale_vector((1/denom), &feature);
		map<unsigned int, double> sub_weights = weights;
		funcs2.scale_vector(2,&sub_weights);
		funcs2.scale_vector(sigma*sigma,&sub_weights);
		map<unsigned int, double> add_result = add_vectors(feature,sub_weights);
		funcs2.scale_vector(-gamma, &add_result);
		weights = add_vectors(weights,add_result);		
	
}








