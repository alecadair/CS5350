/*
 * main.cpp
 *
 *  Created on: Nov 24, 2017
 *      Author: adair
 */

#include <stdlib.h>
#include <iostream>
#include "svm.h"
#include "naive_bayes.h"

int main(){
	SVM svm;
	svm.fill_training_vecs("./data/speeches.train.liblinear",0);
//	for(auto vec : svm.training_vectors[0]){
//		cout << vec.first << " " << vec.second << endl;
//	}
	svm.run_svm(.001,.1);
	svm.test_accuracy("./data/speeches.test.liblinear");
	return 0;
}


