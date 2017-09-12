/*
 * id3.h
 *
 *  Created on: Sep 8, 2017
 *      Author: adair
 */

#ifndef ID3_H_
#define ID3_H_

#include <vector>
#include <string.h>
#include <string>

typedef struct Node{
	std::vector<Node> children;
	int attribute;
	bool is_leaf;
	bool label;
}Node;

class ID3{
public:
	std::vector<std::vector<std::string> > names_vector;
	std::vector<std::vector<bool> > attributes;
	std::vector<bool> labels;
	Node root_node;
	ID3(std::vector<std::vector<std::string> > _names,
			std::vector<std::vector<bool> > _attribute_table,
			std::vector<bool> _labels);
	Node induce_tree(unsigned max_depth);

private:
	Node id3_algorithm(std::vector<std::vector<std::string> > names,
						std::vector<std::vector<bool> > attribute_table,
						std::vector<bool> labels, std::vector<int> _skip_list, unsigned max_depth,
							unsigned current_depth);

	double calculate_info_gain(std::vector<std::vector<bool> > attribute_table,
								std::vector<bool> labels,
								std::vector<int> skip_list, int attribute);

	bool in_skip_list(std::vector<int> skip_list, int attr);
//	double calculate_info_gain(std::vector<std::vector<bool> > attribute_table,
//								std::vector<bool> labels, int attribute);
	double calculate_entropy(std::vector<std::vector<bool> > attribute_table,
									std::vector<bool> labels, bool all_labels,
									std::vector<int> skip_list, int attribute, bool attr_val);

};

#endif /* ID3_H_ */
