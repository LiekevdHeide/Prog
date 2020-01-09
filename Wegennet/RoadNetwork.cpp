#include "RoadNetwork.h"

#include <iostream> //print on screen
#include <fstream> //for reading
#include <vector>
#include <string>
#include <cassert>//assertion of ifstream

using namespace std;

RoadNetwork::RoadNetwork(string inputDoc) {

	ifstream readFrom(inputDoc);
	assert(readFrom && "Wrong network input doc");
	string trash;
	readFrom >> trash;//trash the first line

	readFrom >> vertices;
	readFrom >> numberODpairs;
	readFrom >> touristPercentage;

	vector<size_t> numPaths(numberODpairs, 0);
	for (size_t n = 0; n < numberODpairs; ++n) {
		readFrom >> numPaths[n];
	}
	numberODpaths = numPaths;

	vector<size_t> numPairs(numberODpairs * 2, 0);
	for (size_t n = 0; n < 2 * numberODpairs; ++n) {
			readFrom >> numPairs[n];
	}
	ODpairs = numPairs;

	vector<double> demands(numberODpairs, 0.0);
	for (size_t n = 0; n < numberODpairs; ++n) {
		readFrom >> demands[n];
	}
	ODdemands = demands;

	vector<vector<double>> readMatrix(vertices, vector<double>(vertices, 0.0));
	for (size_t v = 0; v < vertices; ++v) {
		for (size_t w = 0; w < vertices; ++w)
			readFrom >> readMatrix[v][w];
	}
	standardCapacities = readMatrix;

	for (size_t v = 0; v < vertices; ++v)
		for (size_t w = 0; w < vertices; ++w) {
			readFrom >> readMatrix[v][w];
		}
	freeFlowTimes = readMatrix;

	//function to calculate which paths!
	readPaths(readFrom);
	cout << "Created network\n";
}
