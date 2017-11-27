/*
 * naive_bayes.h
 *
 *  Created on: Nov 26, 2017
 *      Author: adair
 */

#ifndef NAIVE_BAYES_H_
#define NAIVE_BAYES_H_

#include <stdlib.h>
#include <map>
#include <vector>
#include <string>

using namespace std;

//struct pair{
//	double postive_prob;
//	double negative_prob;
//}pair;

class Pair{
public:
	double positive_prob;
	double negative_prob;
};

class NaiveBayes {
public:
	NaiveBayes();
	virtual ~NaiveBayes();

	void run_nbayes(string training_file, double smoothing_term, int limit);
	void fill_training_vecs(string filename, char is_test);
	void run_test(string filename, int limit);
	double get_example_from_data(string line,
	map<unsigned int, double>* feat_vec, double* label, char is_test);

	//void calculate_probabilities(double smoothing_term);
	double calculate_probability_feature(unsigned int feature, double label, double smoothing_term, int limit);
private:
	vector<map<unsigned int, double> > training_vectors;
	vector<double> training_labels;
	double label_positive_prob;
	map<unsigned int,Pair> probability_map;
	unsigned int num_features;

};

#endif /* NAIVE_BAYES_H_ */
