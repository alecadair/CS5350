/*
 * nearest_neighbors.cpp
 *
 *  Created on: Nov 13, 2017
 *      Author: adair
 */

#include "nearest_neighbors.h"

#define NUM_FEATURES 17

double KNearestNeighbors::calculate_distance(vector<double> v1, vector<double> v2){
	double distance = 0;
	for(unsigned int i = 0; i < v1.size(); i++){
		distance += ((v2[i] - v1[i])*(v2[i] - v1[i]));
	}
	distance = sqrt(distance);
	return distance;
}


void KNearestNeighbors::fill_training_examples(){
	ifstream tf_stream ("./DatasetRetry/data-splits/data.train");
	FileExtraction extract;
	if(tf_stream.is_open()){
		string line;
		while(getline(tf_stream,line)){
			double label = 0;
			vector<double> example(NUM_FEATURES,0);
			example[example.size() -1] = 1;
			extract.get_example_from_line(line,&example,&label);
			training_examples.push_back(example);
			training_labels.push_back(label);
		}
	}
	tf_stream.close();
}
void KNearestNeighbors::run_knn(){
	double K = 1;
	FileExtraction extract;
	fill_training_examples();
	int size = training_examples.size();

	ifstream tf_stream("./DatasetRetry/data-splits/data.test");
	ifstream name_stream("./DatasetRetry/data-splits/data.test.id");
	cout << "ID,Prediction" << endl;
	unsigned int count = 0;
	double max_distance = DBL_MAX;
	if(tf_stream.is_open() && name_stream.is_open()){
		string line;
		double right = 0, wrong = 0, total = 0;
		while(getline(tf_stream,line)){
			string name;
			getline(name_stream, name);
			double label = 0;
			vector<double> example(NUM_FEATURES, 0);
			example[example.size() - 1] = 1;
			extract.get_example_from_line(line,&example,&label);

			//now have example
			//calculate distance
			double classification = calculate_training_distances(example, K);
			if(classification == label)
				right ++;
			else
				wrong ++;
			total++;

			cout << name <<"," << (int)classification << endl;
			count++;
			//if(count >= 1000){
			//	break;
			//}
		}
		cout << "Accuracy:\t" << (right/total) << endl;
		cout << "Total Tests\t" << total << endl;
		cout << "Total Right\t" << right << endl;
		cout << "Total Wrong\t" << wrong << endl;
	}
	tf_stream.close();
	name_stream.close();
}

double KNearestNeighbors::calculate_training_distances(vector<double> example, double K){
	FileExtraction extract;
	//ifstream tf_stream("./DatasetRetry/data-splits/data.train");
	vector<double> klabels(K,DBL_MAX);
	vector<double> kdistances(K,DBL_MAX);
	//if(tf_stream.is_open()){
		string line;
		//while(getline(tf_stream,line)){
		for(unsigned int l = 0; l < training_labels.size();l ++){
			//double training_label = 0;
			//vector<double> training_example(NUM_FEATURES,0);
			//training_example[training_example.size() -1] = 1;
			//extract.get_example_from_line(line,&training_example,&training_label);
			//double dist = calculate_distance(example,training_example);
			double dist = calculate_distance(example, training_examples[l]);
			for(unsigned i = 0; i < kdistances.size(); i++){
				if(dist <= kdistances[i]){
					//kdistances[i] = dist;
					if(kdistances.size() > 1){
						for(unsigned j = kdistances.size() -1; j > i; j--){
							kdistances[j-1] = kdistances[j];
							klabels[j-1] = klabels[j];
						}
						kdistances[i] = dist;
						klabels[i] = training_labels[l];
						break;
					}
					else{
						kdistances[0] = dist;
						klabels[0] = training_labels[l];
					}

				}
			}
		}
	//}
	//tf_stream.close();
	double negatives = 0, positives = 0;
	for(unsigned i = 0; i < klabels.size(); i++){
		if(klabels[i] == 0)
			negatives ++;
		else
			positives ++;
	}
	double maj_label = 0;
	if(negatives > positives)
		return 0;
	else
		return 1;
}
