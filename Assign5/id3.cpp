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
	double gain = calculate_entropy(attribute_table,labels,attribute);
//
	return gain;
//	double total_entropy = calculate_entropy(attribute_table, labels, 1,
//			attribute, 0);
//	vector<map<unsigned int, double> > attribute_table_truesv;
//	vector<map<unsigned int, double> > attribute_table_falsesv;
//	vector<double> labels_truesv;
//	vector<double> labels_falsesv;
//	for (unsigned int i = 0; i < attribute_table->size(); i++) {
//		map<unsigned int, double> example = attribute_table->at(i);
//		map<unsigned int, double>::iterator ex_iter = example.find(attribute);
//		if (ex_iter != example.end()) {
//			attribute_table_truesv.push_back(example);
//			labels_truesv.push_back(labels->at(i));
//		} else {
//			attribute_table_falsesv.push_back(example);
//			labels_falsesv.push_back(labels->at(i));
//		}
//	}
//	double entropy_true = calculate_entropy(&attribute_table_truesv,
//			&labels_truesv, false, attribute, 1);
//	double entropy_false = calculate_entropy(&attribute_table_falsesv,
//			&labels_falsesv, false, attribute, 0);
//	entropy_true *= (((float) labels_truesv.size()) / ((float) labels->size()));
//	entropy_false *=
//			(((float) labels_falsesv.size()) / ((float) labels->size()));
//	//cout << "Attribute " << attribute << endl;
//	//cout << "entropy false " << entropy_false << endl;
//	//cout << "entropy true " <<  entropy_true << endl;
//	//cout << "total entropy " << total_entropy << endl;
//	double gain = total_entropy - (entropy_true + entropy_false);
	//cout << "gain " << gain << endl;
	//return gain;
}

double ID3::calculate_entropy(
		vector<map<unsigned int, double> >* attribute_table,
		vector<double> *labels, unsigned int attribute) {
	double yesposcount, negposcount = 0;
	double yesnegcount, negnegcount = 0;
	double total_entropy = 0;
	double pos_proportion, neg_proportion = 0;
	double negatives = 0, positives = 0;
	double entropy = 0;
	//if (all_labels) {
//	for (unsigned i = 0; i < labels->size(); i++) {
//		if (labels->at(i) == -1)
//			negatives++;
//		else
//			positives++;
//	}

//	pos_proportion = positives / (negatives + positives);
//	neg_proportion = negatives / (negatives + positives);
	//entropy = -1 * pos_proportion * log2(pos_proportion);
	//entropy -= neg_proportion * log2(neg_proportion);
//
	for(unsigned int i = 0; i < attribute_table->size(); i++){
		map<unsigned int, double>::iterator ex_iter = attribute_table->at(i).find(attribute);
		if(labels->at(i) == 1)
			positives++;
		else
			negatives++;
		if(ex_iter != attribute_table->at(i).end()){
			if(labels->at(i) == 1)
				yesposcount ++;
			else
				yesnegcount ++;
		}else{
			if(labels->at(i) == 1)
				negposcount ++;
			else
				negnegcount++;
		}
	}
	double yestotal = yesposcount + yesnegcount;
	double yesentropy = 0;
	if(yesposcount != 0 && yesnegcount != 0){
		yesentropy = (-1*(yesposcount/yestotal)*log2(yesposcount/yestotal)) - (yesnegcount/yestotal)*log2(yesnegcount/yestotal);
	}
	double noentropy = 0;
	double negtotal = negposcount + negnegcount;
	if(negposcount != 0&& negnegcount != 0){
		noentropy = (-1*(negposcount/negtotal)*log2(negposcount/negtotal)) - (negnegcount/negtotal)*log2(negnegcount/negtotal);
	}
	double poscoef = yestotal/(double)labels->size();
	double negcoef = negtotal/(double)labels->size();
	double gain = (-positives/(positives+negatives))*log2(positives/(positives+negatives)) + (-negatives/(positives+negatives))*log2(negatives/(positives+negatives));

	return gain - ((poscoef*yesentropy)+(negcoef*noentropy));

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

Node ID3::induce_tree(unsigned int max_depth, string training_file) {
	populate_training_data(training_file);
	return id3_algorithm(&training_data, &initial_labels, &initial_attributes, 1);
}

Node ID3::id3_algorithm(vector<map<unsigned int, double> >* examples,
		vector<double>* labels, set<unsigned int>* attributes,
		unsigned int current_depth) {
	Node new_node;
	new_node.is_leaf = 0;
	//check if all labels are the same
	char same_labels = 1;
	if (labels->size() < 3) {
		double maj_lab = majority_label(labels);
		new_node.is_leaf = 1;
		new_node.label = maj_lab;
		return new_node;
	}
	for (unsigned int i = 0; i < labels->size() - 2; i++) {
		if (labels->at(i) != labels->at(i + 1))
			same_labels = 0;
	}
	if (same_labels) {
		new_node.is_leaf = 1;
		new_node.label = labels->at(0);
		return new_node;
	}
	unsigned int max_attribute = 0;
	double max_info_gain = 0;
	set<unsigned int>::iterator attr_iter = attributes->begin();
	attr_iter ++;
	unsigned int counter = 0;
	for (; attr_iter != attributes->end(); attr_iter++) {
		//if(attr_iter == 0)
		//	attr_iter ++;
		double info_gain = calculate_info_gain(examples, labels, *attr_iter);
		cout << info_gain << endl;
		//cout << info_gain << endl;
		if (info_gain >= max_info_gain) {
			max_info_gain = info_gain;
			max_attribute = *attr_iter;
		}
		//cout << "Max attr: " << max_attribute << endl;
		counter++;
		if (counter >= 1000)
			break;
	}
	cout << "Max attribute " << max_attribute << endl;
	new_node.attribute = max_attribute;
	//attributes->erase(max_attribute);
	set<unsigned int> new_attributes = *attributes;
	new_attributes.erase(max_attribute);
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
		//cout << "sv size: " << sv.size() << endl;
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
	if (pos > neg)
		return 1;
	else
		return -1;
}

void ID3::test(string test_file, Node tree){
	ifstream test_stream(test_file);
	string test_line;
	double right = 0, wrong = 0, pos = 0, neg = 0;
	while(getline(test_stream,test_line)){
		map<unsigned int, double> test_ex;
		double test_lab = 0;
		funcs.get_example_from_data(test_line,&test_ex,&test_lab,1);
		Node temp = tree;
		while(temp.is_leaf == 0){
			unsigned int attr = temp.attribute;
			if(test_ex.find(attr) == test_ex.end()){
				temp = temp.children[0];
			}else{
				temp = temp.children[1];
			}
		}
		cout << temp.label << endl;
		double label = temp.label;
		if(label == test_lab)
			right ++;
		else
			wrong++;
		if(label == 1)
			pos ++;
		else
			neg++;
	}
	cout << "Accuracy - " << ((right)/(right+wrong)) << endl;
	cout << "Right - " << right << endl;
	cout << "Wrong - " << wrong << endl;
	cout << "Pos - " << pos << endl;
	cout << "Neg - " << neg << endl;
}
