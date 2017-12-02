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
#include "id3.h"
#include "logistic_regression.h"

int main(){
	//SVM svm;
	//svm.fill_training_vecs("./data/speeches.train.liblinear",0);
//	for(auto vec : svm.training_vectors[0]){
//		cout << vec.first << " " << vec.second << endl;
//	}
	//svm.run_svm(.001,.1);
	//svm.test_accuracy("./data/speeches.test.liblinear");

	//NaiveBayes bayes;
	//bayes.run_nbayes("./data/speeches.train.liblinear",10, 30000);
	//bayes.run_test("./data/speeches.test.liblinear", 30000);
	//LogisticRegression reg;
	//reg.tradeoff = 1;
	//reg.learning_rate = 1;

	//reg.train_classifier("./data/speeches.train.liblinear");
	//reg.test("./data/speeches.test.liblinear");
	//reg.train_classifier("../Assign2/Dataset/phishing.train");
	//reg.test("../Assign2/Dataset/phishing.test");

	ID3 id3;
	Node root = id3.induce_tree(0,"./data/CVSplits/training00.data");
	id3.test("./data/CVSplits/training01.data", root);
	return 0;
}


