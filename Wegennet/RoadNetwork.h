
#pragma once
#include <vector>
using namespace std;
class RoadNetwork
{
public:
	//constructor!

	size_t vertices; //vertices in the network

	//OD pairs
	size_t numberODpairs;
	vector<size_t> ODpairs;

	//OD demands
	vector<double> ODdemands; //demand per OD pair

	//paths per OD pair
	vector<size_t> numberODpaths; // how many paths per OD pair (also size of the next..?)
	vector<vector<size_t>> ODpaths;//which path, order of vertices
	//
	vector<vector<double>> standardCapacities;
	vector<vector<double>> freeFlowTimes;

	double TravelTimeRoad(double capacity, double freeFlow, double actualFlow) {//returns travel time on an arc, given capacity, freeFlow, actualFlow
		return(freeFlow * (1 + 0.15 * pow(actualFlow / freeFlow, 4)));
	}
};

