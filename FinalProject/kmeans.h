/*
 * kmeans.h
 *
 *  Created on: Dec 12, 2017
 *      Author: alec
 */

#ifndef KMEANS_H_
#define KMEANS_H_

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Point{
public:
	vector<double> data;
	int cluster;
	double label;
	int points_in_cluster;
	//unsigned int dimensionality

};
class KMeans {
public:
	vector<Point> means;
	vector<Point> training_data;
	vector<double> training_labels;
	KMeans();
	virtual ~KMeans();
	void run_kmeans(string filename, int K, int num_iterations);
	double calculate_distance(vector<double> v1, vector<double> v2);
	int classify_test_example(Point example);
	void classify_cluster();
	void test_kmeans(string filename);
	void initialize_means(string filename, int k);
	void update_means();
	void get_example_from_line(string line, vector<double>* result, double* label);

};

#endif /* KMEANS_H_ */
