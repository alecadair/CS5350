/*
 * id3.h
 *
 *  Created on: Dec 1, 2017
 *      Author: adair
 */

#ifndef ID3_H_
#define ID3_H_
#include <vector>
#include <string>
#include <map>
#include <set>
//#include <bool.h>

#include "vector_funcs.h"
using namespace std;

typedef struct Node{
	vector<Node> children;
	unsigned int attribute;
	bool is_leaf;
	double label;
}Node;

class ID3 {
public:
	ID3();
	virtual ~ID3();
	vector<double> initial_labels;
	vector<map<unsigned int, double> > training_data;
	vector<double> average_attributes;
	Node root_node;
	set<unsigned int> initial_attributes;
	void populate_training_data(string);
	//VectorFuncs funcs;
	VectorFuncs funcs;
	double get_example_from_data(string, map<unsigned int, double>*, double*, char);
	double calculate_info_gain(vector<map<unsigned int, double> >* attribute_table,
	                vector<double>* labels, unsigned int attribute);
	double calculate_entropy(vector<map<unsigned int, double> >* attribute_table, vector<double>* labels, unsigned int attribute);
	Node induce_tree(unsigned int max_depth, string training_file);
	Node id3_algorithm(vector<map<unsigned int, double> >* examples, vector<double>* labels, set<unsigned int>* attributes, unsigned int current_depth);
	double majority_label(vector<double>* labels);
	void test(string test_file, Node tree);
	void find_averages();
};
#endif /* ID3_H_ */
