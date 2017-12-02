/*
 * bagged_forest.cpp
 *
 *  Created on: Dec 2, 2017
 *      Author: adair
 */

#include "bagged_forest.h"
#include <time.h>
#include <random>
#include <fstream>
#include <iostream>

BaggedForest::BaggedForest() {
	// TODO Auto-generated constructor stub

}

BaggedForest::~BaggedForest() {
	// TODO Auto-generated destructor stub

}
double BaggedForest::test_one_example(map<unsigned int, double> test_feature,
		double label, Node tree) {
	//map<unsigned int, double> test_ex;
	//double test_lab = 0;
	//funcs.get_example_from_data(test_line, &test_ex, &test_lab, 1);
	Node temp = tree;
	while (temp.is_leaf == 0) {
		unsigned int attr = temp.attribute;
		if (test_feature.find(attr) == test_feature.end()) {
			temp = temp.children[0];
		} else {
			temp = temp.children[1];
		}
	}
	double test_label = temp.label;
	return test_label;
}

void BaggedForest::run_bagged_forest(string filename) {
	ID3 id3;
	id3.populate_training_data(filename);
	unsigned int num_trees = 13;
	//vector<id3> trees;
	srand(time(NULL));
	for (unsigned i = 0; i < num_trees; i++) {
		cout << "Building tree " << i <<endl;
		vector<map<unsigned int, double> > random_examples;
		vector<double> rand_labels;
		//vector<double>
		for (unsigned j = 0; j < 100; j++) {
			unsigned int rand_ind = rand();
			rand_ind = rand_ind % (id3.training_data.size());
			double label = id3.initial_labels.at(rand_ind);
			rand_labels.push_back(label);
			random_examples.push_back(id3.training_data.at(rand_ind));
		}
		ID3 forest;
		trees.push_back(
				forest.id3_algorithm(&random_examples, &rand_labels,
						&(id3.initial_attributes), 0));
	}
}

void BaggedForest::test(string filename) {
	ifstream test_stream(filename);
	ID3 id;
	string test_line;
	double right = 0, wrong = 0, pos = 0, neg = 0;
	while (getline(test_stream, test_line)) {
		map<unsigned int, double> test_ex;
		double test_lab = 0;
		vector<double> tree_labels;
		funcs.get_example_from_data(test_line, &test_ex, &test_lab, 1);
		for (unsigned i = 0; i < trees.size(); i++) {
			tree_labels.push_back(
					test_one_example(test_ex, test_lab, trees[i]));
		}
		double maj = id.majority_label(&tree_labels);
		if (maj == test_lab)
			right++;
		else
			wrong++;
		if (maj == 1)
			pos++;
		else
			neg++;
	}
	cout << "Accuracy - " << ((right) / (right + wrong)) << endl;
	cout << "Right - " << right << endl;
	cout << "Wrong - " << wrong << endl;
	cout << "Pos - " << pos << endl;
	cout << "Neg - " << neg << endl;
}

