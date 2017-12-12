/*
 * bagged_forest.h
 *
 *  Created on: Dec 2, 2017
 *      Author: adair
 */

#ifndef BAGGED_FOREST_H_
#define BAGGED_FOREST_H_
#include "id3.h"
#include "stdlib.h"
#include "vector_funcs.h"




class BaggedForest {
public:
	//vector<Node> trees;
	vector<ID3> forest;
	VectorFuncs funcs;
	BaggedForest();
	virtual ~BaggedForest();
	void run_bagged_forest(string filename);
	double test_one_example(map<unsigned int, double> test_feature,double label, ID3 tree);
	void test(string filename);
};

#endif /* BAGGED_FOREST_H_ */
