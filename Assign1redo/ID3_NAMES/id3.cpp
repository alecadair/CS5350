/*
 * id3.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: adair
 */

#include "id3.h"
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

ID3::ID3(vector<vector<string> > _names,
		vector<vector<bool> > _attribute_table, vector<bool> _labels) {
	names_vector = _names;
	attributes = _attribute_table;
	labels = _labels;
	root_node.attribute = ~0;
	root_node.is_leaf = false;
}

Node ID3::induce_tree(unsigned max_depth) {
	vector<int> empty_skiplist;
	Node tree = id3_algorithm(names_vector, attributes, labels, empty_skiplist,
			max_depth, 0);
	return tree;
}

Node ID3::id3_algorithm(vector<vector<string> > names,
		vector<vector<bool> > attribute_table, vector<bool> labels,
		vector<int> _skip_list, unsigned max_depth, unsigned current_depth) {

	vector<int> skip_list = _skip_list;

	//check to see if all examples have same label
	Node new_root;
	new_root.is_leaf = false;
	new_root.label = false;
	if (current_depth == max_depth) {
		int pos_count = 0, neg_count = 0;
		for (unsigned i = 0; i < names.size(); i++) {
			if (labels[i] == true)
				pos_count++;
			else
				neg_count++;
		}
		if (pos_count > neg_count)
			new_root.label = true;
		else
			new_root.label = false;
		new_root.is_leaf = true;
		return new_root;

	}
	bool same_labels = true;
	for (unsigned i = 1; i < labels.size(); i++) {
		if (labels[i] != labels[i - 1])
			same_labels = false;
	}
	//Node new_root;
	new_root.is_leaf = false;
	if (same_labels) {
		new_root.is_leaf = true;
		new_root.label = labels[0];
		root_node = new_root;
		//root_node.is_leaf = true;
		//root_node.label = _labels[0];
		return new_root;
	}
	//calculate attribute with highest info gain
	int highest_gain_attr = 0;
	double highest_gain = 0;
	if (skip_list.size() >= 8) {
		new_root.is_leaf = true;
		int pos_count = 0, neg_count = 0;
		for (unsigned i = 0; i < names.size(); i++) {
			if (labels[i] == true)
				pos_count++;
			else
				neg_count++;
		}
		if (pos_count > neg_count)
			new_root.label = true;
		else
			new_root.label = false;
		return new_root;
	}
	for (unsigned i = 0; i < attribute_table[0].size(); i++) {
		if (in_skip_list(skip_list, i) == false) {
			double temp = calculate_info_gain(attribute_table, labels,
					skip_list, i);
			if (temp >= highest_gain) {
				highest_gain = temp;
				highest_gain_attr = (int) i;
			} else {

			}
		}
	}
	new_root.attribute = highest_gain_attr;
	skip_list.push_back(highest_gain_attr);
	//add new tree branch for each value of highest gain attribute
	for (unsigned i = 0; i < 2; i++) {
		Node new_node;
		//new_root.children.push_back(new_node);
		vector<vector<string> > names_sv;
		vector<vector<bool> > attribute_table_sv;
		vector<bool> labels_sv;
		unsigned sv_size = 0;
		//iterate through examples check for attribute values and add to Sv
		for (unsigned j = 0; j < names.size(); j++) {
			if (attribute_table[j][highest_gain_attr] == (bool) i) {
				names_sv.push_back(names[j]);
				attribute_table_sv.push_back(attribute_table[j]);
				labels_sv.push_back(labels[j]);
				sv_size++;
			}
		}
		if (sv_size == 0) {
			//find common label of S
			unsigned pos_count = 0, neg_count = 0;
			for (unsigned j = 0; j < names.size(); j++) {
				if (labels[j] == true)
					pos_count++;
				else
					neg_count++;
			}
			if (pos_count > neg_count)
				new_node.label = true;
			else
				new_node.label = false;
			new_node.is_leaf = true;
			new_root.children.push_back(new_node);
		} else {
			new_root.children.push_back(
					id3_algorithm(names_sv, attribute_table_sv, labels_sv,
							skip_list, max_depth, current_depth + 1));
		}
	}
	return new_root;
}

bool ID3::in_skip_list(vector<int> skip_list, int attr) {
	if (skip_list.empty())
		return false;
	for (unsigned i = 0; i < skip_list.size(); i++) {
		if (skip_list[i] == attr)
			return true;
	}
	return false;
}

double ID3::calculate_info_gain(vector<vector<bool> > attribute_table,
		vector<bool> labels, vector<int> skip_list, int attribute) {
	double total_entropy = calculate_entropy(attribute_table, labels, true,
			skip_list, attribute, false);
	vector<vector<bool> > attribute_table_truesv;
	vector<vector<bool> > attribute_table_falsesv;
	vector<bool> labels_truesv;
	vector<bool> labels_falsesv;
	for (unsigned i = 0; i < attribute_table.size(); i++) {

		if (attribute_table[i][attribute] == true) {
			attribute_table_truesv.push_back(attribute_table[i]);
			labels_truesv.push_back(labels[i]);
		} else {
			attribute_table_falsesv.push_back(attribute_table[i]);
			labels_falsesv.push_back(labels[i]);
		}
	}

	double entropy_true = calculate_entropy(attribute_table_truesv,
			labels_truesv, false, skip_list, attribute, true);

	double entropy_false = calculate_entropy(attribute_table_falsesv,
			labels_falsesv, false, skip_list, attribute, false);
	entropy_true *= (((float) labels_truesv.size()) / ((float) labels.size()));
	entropy_false
			*= (((float) labels_falsesv.size()) / ((float) labels.size()));
	double gain = total_entropy - (entropy_true + entropy_false);
	return gain;
}

double ID3::calculate_entropy(vector<vector<bool> > attribute_table,
		vector<bool> labels, bool all_labels, vector<int> skip_list,
		int attribute, bool attribute_val) {

	double positives, negatives = 0;
	double positive_proportion, negative_proportion;
	if (all_labels) {
		//double positives, negatives = 0;
		for (unsigned i = 0; i < labels.size(); i++) {
			if (labels[i] == false)
				negatives++;
			else
				positives++;
		}
		if (positives == 0 || negatives == 0) {
			return 0;
		}
		double positive_proportion = positives / (negatives + positives);
		double negative_proportion = negatives / (negatives + positives);
		double h_of_S = -1 * positive_proportion * log2(positive_proportion);
		h_of_S -= (negative_proportion * log2(negative_proportion));
		return h_of_S;
	}
	double examples_with_val = 0;

	//find total examples with attribute equal to attribute_val
	// and proportions of labels
	for (unsigned i = 0; i < attribute_table.size(); i++) {
		if (attribute_table[i][attribute] == attribute_val) {
			examples_with_val++;
			if (labels[i] == true)
				positives++;
			else
				negatives++;
		}
	}
	if (positives == 0 || negatives == 0)
		return 0;
	positive_proportion = positives / examples_with_val;
	negative_proportion = negatives / examples_with_val;
	double h_of_S = -1 * positive_proportion * log2(positive_proportion);
	h_of_S -= (negative_proportion * log2(negative_proportion));
	return h_of_S;
}

