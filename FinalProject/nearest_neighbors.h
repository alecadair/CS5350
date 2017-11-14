/*
 * nearest_neighbors.h
 *
 *  Created on: Nov 13, 2017
 *      Author: adair
 */

#ifndef NEAREST_NEIGHBORS_H_
#define NEAREST_NEIGHBORS_H_

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cfloat>
#include <math.h>
#include "file_extraction.h"

using namespace std;

class KNearestNeighbors{
public:

	vector<vector<double> > training_examples;
	vector<double> training_labels;
	double calculate_distance(vector<double>, vector<double>);
	double calculate_training_distances(vector<double> example, double);
	void run_knn();
	void fill_training_examples();


};



#endif /* NEAREST_NEIGHBORS_H_ */
