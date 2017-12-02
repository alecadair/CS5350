/*
 * id3.cpp
 *
 *  Created on: Dec 1, 2017
 *      Author: adair
 */

#include "id3.h"
#include <stdlib.h>
#include <iostream>
#include <map>
#include <fstream>
#include "vector_funcs.h"
#include "math.h"

ID3::ID3() {
	// TODO Auto-generated constructor stub

}

ID3::~ID3() {
	// TODO Auto-generated destructor stub
}

double ID3::calculate_info_gain(
		vector<map<unsigned int, double> >* attribute_table,
		vector<double>* labels, unsigned int attribute) {

	double total_entropy = calculate_entropy(attribute_table, labels, 1,
			attribute, 0);
	vector<map<unsigned int, double> > attribute_table_truesv;
	vector<map<unsigned int, double> > attribute_table_falsesv;
	vector<double> labels_truesv;
	vector<double> labels_falsesv;
	for (unsigned int i = 0; i < attribute_table->size(); i++) {
		map<unsigned int, double> example = attribute_table->at(i);
		map<unsigned int, double>::iterator ex_iter = example.find(attribute);
		if (ex_iter != example.end()) {
			attribute_table_truesv.push_back(example);
			labels_truesv.push_back(labels->at(i));
		} else {
			attribute_table_falsesv.push_back(example);
			labels_falsesv.push_back(labels->at(i));
		}
	}
	double entropy_true = calculate_entropy(&attribute_table_truesv,
			&labels_truesv, false, attribute, 1);
	double entropy_false = calculate_entropy(&attribute_table_falsesv,
			&labels_falsesv, false, attribute, 0);
	entropy_true *= (((float) labels_truesv.size()) / ((float) labels->size()));
	entropy_false *=
			(((float) labels_falsesv.size()) / ((float) labels->size()));
	//cout << "Attribute " << attribute << endl;
	//cout << "entropy false " << entropy_false << endl;
	//cout << "entropy true " <<  entropy_true << endl;
	//cout << "total entropy " << total_entropy << endl;
	double gain = total_entropy - (entropy_true + entropy_false);
	//cout << "gain " << gain << endl;
	return gain;
}

double ID3::calculate_entropy(
		vector<map<unsigned int, double> >* attribute_table,
		vector<double> *labels, char all_labels, unsigned int attribute,
		double attribute_val) {
	double positives, negatives = 0;
	double pos_proportion, neg_proportion = 0;
	double entropy = 0;
	//if (all_labels) {
	for (unsigned i = 0; i < labels->size(); i++) {
		if (labels->at(i) == -1)
			negatives++;
		else
			positives++;
	}
	if (positives == 0 || negatives == 0)
		return 0;
	pos_proportion = positives / (negatives + positives);
	neg_proportion = negatives / (negatives + positives);
	entropy = -1 * pos_proportion * log2(pos_proportion);
	entropy -= neg_proportion * log2(neg_proportion);
	return entropy;
	//}
	/*double examples_with_val = 0;
	 //find total examples with attribute equal to attribute_val
	 //and proportions of labels
	 for (unsigned i = 0; i < attribute_table->size(); i++) {
	 map<unsigned int, double>::iterator example_iter;
	 map<unsigned int, double> example = attribute_table->at(i);
	 example_iter = example.find(attribute);
	 if (attribute_val == 0) {
	 //if (example_iter == example.end()) {
	 //	examples_with_val++;
	 if (labels->at(i) == 1)
	 positives++;
	 else
	 negatives++;
	 //}
	 } else {
	 //if (example_iter != example.end()) {
	 //	examples_with_val++;
	 if (labels->at(i) == 1)
	 positives++;
	 else
	 negatives++;
	 //}
	 }
	 }
	 if (positives == 0 || negatives == 0)
	 return 0;
	 pos_proportion = positives / attribute_table->size();
	 neg_proportion = negatives / attribute_table->size();
	 entropy = -1 * pos_proportion * log2(pos_proportion);
	 entropy -= neg_proportion * log2(neg_proportion);
	 return entropy;*/
}

void ID3::populate_training_data(string filename) {
	fstream file_stream(filename);
	string line;
	while (getline(file_stream, line)) {
		map<unsigned int, double> example;
		double label = 0;
		funcs.get_example_from_data(line, &example, &label, 0);
		training_data.push_back(example);
		map<unsigned int, double>::iterator ex_iter = example.begin();
		for (; ex_iter != example.end(); ex_iter++)
			initial_attributes.insert(ex_iter->first);

		initial_labels.push_back(label);
	}
}

void ID3::induce_tree(unsigned int max_depth, string training_file) {
	populate_training_data(training_file);
	id3_algorithm(&training_data, &initial_labels, &initial_attributes, 1);
}

Node ID3::id3_algorithm(vector<map<unsigned int, double> >* examples,
		vector<double>* labels, set<unsigned int>* attributes,
		unsigned int current_depth) {
	Node new_node;
	//check if all labels are the same
	char same_labels = 1;
	if (labels->size() < 3) {
		double maj_lab = majority_label(labels);
		new_node.is_leaf = true;
		new_node.label = maj_lab;
		return new_node;
	}
	for (unsigned int i = 0; i < labels->size() - 2; i++) {
		if (labels->at(i) != labels->at(i + 1))
			same_labels = 0;
	}
	if (same_labels) {
		new_node.is_leaf = true;
		new_node.label = labels->at(0);
		return new_node;
	}
	unsigned int max_attribute = 0;
	double max_info_gain = 0;
	set<unsigned int>::iterator attr_iter = attributes->begin();
	unsigned int counter = 0;
	for (; attr_iter != attributes->end(); attr_iter++) {
		double info_gain = calculate_info_gain(examples, labels, *attr_iter);
		//cout << info_gain << endl;
		if (info_gain >= max_info_gain) {
			max_info_gain = info_gain;
			max_attribute = *attr_iter;
		}
		//cout << "Max attr: " << max_attribute << endl;
		counter++;
		if (counter >= 300)
			break;
	}
	cout << "Max attribute " << max_attribute << endl;
	new_node.attribute = max_attribute;
	attributes->erase(max_attribute);
	for (unsigned i = 0; i < 2; i++) {
		Node child;
		vector<map<unsigned int, double> > sv;
		vector<double> labels_sv;
		for (unsigned j = 0; j < examples->size(); j++) {
			if (i == 0) {
				if (examples->at(j).find(max_attribute)
						== examples->at(j).end()) {
					sv.push_back(examples->at(j));
					labels_sv.push_back(labels->at(j));
				}
			} else {
				if (examples->at(j).find(max_attribute)
						!= examples->at(j).end()) {
					sv.push_back(examples->at(j));
					labels_sv.push_back(labels->at(j));
				}
			}
		}
		cout << "sv size: " << sv.size() << endl;
		//if sv is empty create l or past current depth
		if (sv.size() == 0 || current_depth >= 3) {
			child.is_leaf = true;
			double maj_lab = majority_label(&labels_sv);
			child.label = maj_lab;
			new_node.children.push_back(child);
			//return new_node;
		} else {
			new_node.children.push_back(id3_algorithm(&sv, &labels_sv, attributes,
					current_depth + 1));

		}
	}
	return new_node;
}
double ID3::majority_label(vector<double>* labels) {
	double pos = 0, neg = 0;
	for (unsigned i = 0; i < labels->size(); i++) {
		if (labels->at(i) == 1)
			pos++;
		else
			neg++;
	}
	if (pos >= neg)
		return 1;
	else
		return -1;
}

