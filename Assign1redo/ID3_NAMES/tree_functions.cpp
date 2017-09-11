/*
 * tree_functions.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: adair
 */
#include "tree_functions.h"

using namespace std;

unsigned TreeFunctions :: get_depth_of_tree(Node node){
	if(node.is_leaf){
		return 0;
	}
	vector<unsigned> depths;
	for(unsigned i = 0; i < node.children.size(); i ++){
		depths.push_back(get_depth_of_tree(node.children[i]));
	}
	unsigned largest_depth = 0;
	for(unsigned i = 0; i < depths.size(); i++){
		if(depths[i] > largest_depth)
			largest_depth = depths[i];
	}
	return largest_depth + 1;
}


bool TreeFunctions :: calculate_label(Node tree, vector<string> name, vector<bool> attributes){
	if(tree.is_leaf)
		return tree.label;
	bool path = attributes[tree.attribute];
	bool label = false;
	if(path)
		label = calculate_label(tree.children[0], name,attributes);
	else
		label = calculate_label(tree.children[1], name, attributes);
	return label;
}
