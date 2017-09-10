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

vector<vector<bool> > attribute_table;
vector<vector<string> >		  names;
vector<bool>		  labels;


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
	for(unsigned i = 0; i < names.size(); i++){
		vector<bool> instance;
		instance.push_back(a0(names[i]));
		instance.push_back(a1(names[i]));
		instance.push_back(a2(names[i]));
		instance.push_back(a3(names[i]));
		instance.push_back(a4(names[i]));
		instance.push_back(a5(names[i]));
		instance.push_back(a6(names[i]));
		instance.push_back(a7(names[i]));

		attribute_table.push_back(instance);
	}
}

void populate_table(){
	string line;
	const char* file_name =  "./Updated_Dataset/Updated_CVSplits/updated_training00.txt";
	ifstream training_file(file_name);
	if(training_file.is_open()){
		//long line_counter = 0;
		while(getline(training_file,line)){
			stringstream line_stream(line);
			vector<string> tokens;
			//long line_counter = 0;
			string label;
			line_stream >> label;
			if(label == "+")
				labels.push_back(true);
			else
				labels.push_back(false);
			//split string by whitespace
			for(string s; line_stream >> s;){
				tokens.push_back(s);
			//	cout << s << endl;
			}
			names.push_back(tokens);
			//cout << line << "\n" << endl;
		}
		//labels and names are now filled
		//fill attribute table
		populate_attribute_table();
	}
}

int main(){
	populate_table();
	ID3 id3(names,attribute_table,labels);
	Node tree = id3.induce_tree();
	int i = 0;
	i++;
	i--;
	return i;
}
