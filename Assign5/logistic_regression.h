/*
 * logistic_regression.h
 *
 *  Created on: Nov 28, 2017
 *      Author: adair
 */

#ifndef LOGISTIC_REGRESSION_H_
#define LOGISTIC_REGRESSION_H_

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <map>
//#include "svm.h"
//#include "naive_bayes.h"
#include "vector_funcs.h"

using namespace std;
class LogisticRegression {
public:
	map<unsigned int, double> weights;
	double learning_rate;
	double tradeoff;
	LogisticRegression();
	void update_weights(map<unsigned int, double> feature, double label, 
							double gamma, double sigma);
	void cross_validation();
	virtual ~LogisticRegression();
	void test(string filename);
	void train_classifier(string filename);
	VectorFuncs funcs;
	map<unsigned int, double> add_vectors(map<unsigned int, double> v1,
							map<unsigned int, double> v2);	
};

#endif /* LOGISTIC_REGRESSION_H_ */
