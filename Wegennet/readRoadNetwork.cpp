#include "RoadNetwork.h"


#include <iostream> //print on screen
#include <fstream> //for reading
#include <vector>
#include <string>
#include <cassert>//assertion of ifstream

void readRoadNetwork(string inputDoc, RoadNetwork &Instance) {
	ifstream readFrom(inputDoc);
	assert(readFrom && "Wrong MDP input doc");
	string trash;
	readFrom >> trash;//trash the first line

	readFrom >> Instance.vertices;

	vector<vector<double>> readMatrix(Instance.vertices, vector<double>(Instance.vertices, 0.0)) ;
	for (size_t v = 0; v < Instance.vertices; ++v) {
		for(size_t w = 0; w < Instance.vertices; ++w)
			readFrom >> readMatrix[v][w];
	}
	Instance.standardCapacities = readMatrix;

	for(size_t v=0; v < Instance.vertices; ++v)
		for (size_t w = 0; w < Instance.vertices; ++w) {
			readFrom >> readMatrix[v][w];
		}
	Instance.freeFlowTimes = readMatrix;

	


}