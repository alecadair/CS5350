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

using namespace std;

class TreeFunctions{

public:
	unsigned get_depth_of_tree(Node node);
	bool calculate_label(vector<string> name);
};

#endif /* TREE_FUNCTIONS_H_ */
