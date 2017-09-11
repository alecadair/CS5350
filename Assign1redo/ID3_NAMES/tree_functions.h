/*
 * tree_functions.h
 *
 *  Created on: Sep 10, 2017
 *      Author: adair
 */

#ifndef TREE_FUNCTIONS_H_
#define TREE_FUNCTIONS_H_

#include <stdlib.h>
#include "id3.h"
#include <vector>
#include <string.h>

using namespace std;

class TreeFunctions{

public:
	unsigned get_depth_of_tree(Node node);
	//bool calculate_label(Node node, vector<string> name, vector<bool> attributes);
	bool calculate_label(Node node, std::vector<std::string> name, std::vector<bool> attributes);

};

#endif /* TREE_FUNCTIONS_H_ */
