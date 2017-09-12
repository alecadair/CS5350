/*

 * main.c
 *
 *  Created on: Sep 8, 2017
 *      Author: adair
 */
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include "id3.h"
#include "tree_functions.h"


/*
 * Attributes
 * 0 first longer than last
 * 1 has middle name
 * 2 first name start and end with same letter
 * 3 first name alphabetically before last
 * 4 2nd letter of first name a vowel
 * 5 number of letter of last name even
 * 6 name has special characters
 * 7 first letter of first and last name are the same
 */

#define NUM_ATTRIBUTES 8

using namespace std;
//

class AttributeNode{
	char attribute;
	vector<AttributeNode> branches;
};

class DecisionTree{
	AttributeNode root_node;
	AttributeNode induce_tree();
};

AttributeNode DecisionTree::induce_tree(){
	AttributeNode node;
	return node;
}
/*First name longer than last*/
bool a0(vector<string> name){
	int firstname_size = name.at(0).size();
	int lastname_size =  name.at(name.size() - 1).size();
	if(firstname_size > lastname_size)
		return true;
	else
		return false;
}

/*has middle name*/
bool a1(vector<string> name){
	if(name.size() > 2)
		return true;
	else
		return false;
}

/*first name first letter and last letter are the same*/
bool a2(vector<string> name){
	string first_name = name.at(0);
	char first = first_name[0];
	char last = first_name[first_name.size()-1];
	if(first == last)
		return true;
	return false;
}

/*first name comes before last name alphabetically*/
bool a3(vector<string> name){
	string first_name = name.at(0);
	string last_name = name.at(name.size() -1);
	if(strcmp(first_name.c_str(),last_name.c_str()) < 0)
		return true;
	return false;
}

/*second letter of first name is a vowel*/
bool a4(vector<string> name){
	char vowels[] = {'a','e','i','o','u'};
	if(name.at(0).size() < 2){
		return false;
	}
	char second_letter = name.at(0).at(1);
	second_letter = tolower(second_letter);
	for(int i = 0; i < 5; i++){
		if(second_letter == vowels[i])
			return true;
	}
	return false;
}

/*last name cardinality is even*/
bool a5(vector<string> name){
	if(name.at(name.size()-1).size()%2 == 0)
		return true;
	else
		return false;
}

/*name has special chars*/
bool a6(vector<string> name){
	string name_temp;
	for(unsigned i = 0; i < name.size(); i ++){
		name_temp = name.at(i);
		for(unsigned j = 0; j < name_temp.size(); j++){
			if(!isalpha(name_temp[j]))
				return false;
		}
	}
	return true;
}

/*first letter of first and last name are the same*/
bool a7(vector<string> name){
	char first_name_letter = name.at(0).at(0);
	char last_name_letter = name.at(name.size() -1).at(0);
	first_name_letter = tolower(first_name_letter);
	last_name_letter = tolower(last_name_letter);
	if(first_name_letter == last_name_letter)
		return true;
	else
		return false;

}

void populate_attribute_table(){

}

//returns test file labels
vector<bool> populate_table(vector<vector<string> >* names, vector<vector<bool> >* attribute_table,
										vector<bool>* labels, const char* file_name){
	string line;
	string label;
	vector<bool> test_labels;
	ifstream training_file(file_name);
	//ifstream test_file_stream(file_name);
	if(training_file.is_open() /*&& test_file_stream.is_open()*/){
		//long line_counter = 0;
		while(getline(training_file,line)){
			stringstream line_stream(line);
			vector<string> tokens;
			//long line_counter = 0;
			string label;
			line_stream >> label;
			if(label == "+")
				labels->push_back(true);
			else
				labels->push_back(false);
			//split string by whitespace
			for(string s; line_stream >> s;){
				tokens.push_back(s);
				//cout << s << " ";
			//	cout << s << endl;
			}
			//cout << "\n";
			names->push_back(tokens);

			//cout << line << "\n" << endl;
		}

		//labels and names are now filled
		//fill attribute table
		//populate_attribute_table();
		for(unsigned i = 0; i < names->size(); i++){
			vector<bool> instance;
			//cout << "\n";
			instance.push_back(a0(names->at(i)));
			instance.push_back(a1(names->at(i)));
			instance.push_back(a2(names->at(i)));
			instance.push_back(a3(names->at(i)));
			instance.push_back(a4(names->at(i)));
			instance.push_back(a5(names->at(i)));
			instance.push_back(a6(names->at(i)));
			instance.push_back(a7(names->at(i)));
			attribute_table->push_back(instance);
			for(unsigned j = 0; j < names->at(i).size(); j++){
			//		cout << names->at(i).at(j) << " ";
			}
		}
	}
	return test_labels;
}

void test_tree(Node tree){

}

int main(){
	vector<vector<string> > names;
	vector<vector<bool> > attribute_table;
	vector<bool> labels;
	//unsigned max_depth = 4;
	populate_table(&names, &attribute_table, &labels, "./Updated_Dataset/updated_train.txt");
	ID3 id3(names,attribute_table,labels);
	Node tree = id3.induce_tree(10);
	TreeFunctions tree_functions;
	unsigned depth_of_tree = tree_functions.get_depth_of_tree(tree);
	vector<vector<string> > test_names;
	vector<vector<bool> > test_attributes;
	vector<bool> test_labels;

	populate_table(&test_names,&test_attributes,&test_labels,"./Updated_Dataset/updated_test.txt");

	vector<bool> experiment_labels;
	cout << "tree depth: " << depth_of_tree << endl;
	for(unsigned i = 0; i < test_names.size(); i ++){
		vector<string> test_name = test_names[i];
		vector<bool> test_attribute = test_attributes[i];
		bool label = tree_functions.calculate_label(tree,test_name,test_attribute);
		experiment_labels.push_back(label);
	}
	float correct_labels = 0, incorrect_labels = 0;
	for(unsigned i = 0; i < experiment_labels.size(); i++){
		if(experiment_labels[i] == test_labels[i])
			correct_labels ++;
		else
			incorrect_labels ++;
	}
	float correct_percent = correct_labels/(correct_labels + incorrect_labels);
	float incorrect_percent = 1 - correct_percent;
	correct_percent *= 100; incorrect_percent *= 100;
	cout << "Max Depth:\t" << depth_of_tree << endl;
	cout << "Correct Percentage:\t" << correct_percent << "\nIncorrect Percentage:\t" << incorrect_percent<< endl;

	/*
	 * 4-Fold cross validation
	 */
	unsigned max_depths[] = {1,2,3,4,5};
	const char* file_names[] = {"./Updated_Dataset/CVSplits/updated_training00.txt",
								"./Updated_Dataset/CVSplits/updated_training01.txt",
								"./Updated_Dataset/CVSplits/updated_training02.txt",
								"./Updated_Dataset/CVSplits/updated_training03.txt"};
	//create data tables for different files
	vector<vector<string> > all_names[4];
	vector<vector<bool> >   all_attribute_tables[4];
	vector<bool> all_labels[4];
	//populate tables
	for(unsigned i = 0; i < 4; i ++){
		populate_table(&all_names[i],&all_attribute_tables[i], &all_labels[i],file_names[i]);
	}

	/*
	 * Run 4-fold cross validation
	 */

	for(unsigned i = 0; i < 4; i++){
		unsigned max_depth = max_depths[i];
		ID3 test_id3(all_names[i],all_attribute_tables[i],all_labels[i]);
		Node test_node = test_id3.induce_tree(max_depth);

		//concatenate test data
		vector<vector<string> > concat_names;
		vector<vector<bool> >   concat_attributes;
		vector<bool>            concat_labels;
		for(unsigned j = 0; j < 4; j++){
			if(j != i){
				for(unsigned k = 0; k < all_labels[j].size(); k ++){
					concat_names.push_back(all_names[j].at(k));
					concat_attributes.push_back(all_attribute_tables[j].at(k));
					concat_labels.push_back(all_labels[j].at(k));
				}
			}
		}

		//test accuracy
		double total_tests = 0, total_positives = 0, total_negatives = 0;
		for(unsigned j = 0; j < concat_labels.size(); j++){
			vector<string> exp_name = concat_names.at(i);
			vector<bool>   exp_attr = concat_attributes.at(i);
			bool label = tree_functions.calculate_label(test_node,exp_name,exp_attr);
			if(label == concat_labels.at(i))
				total_positives ++;
			else
				total_negatives ++;
			total_tests ++;
		}
		double accuracy = total_positives/(total_tests);
		double missed = 1 - accuracy;

		cout << "Training on data set training 0" << i << endl;
		cout << "Maximum Depth\t" << max_depth << endl;

		cout << "Accuracy:\t" << accuracy << endl;
		cout << "Inaccuracy:\t" << missed << endl;

	}
	return 0;
}










