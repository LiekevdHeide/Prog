
#pragma once
#include <vector>
using namespace std;
class RoadNetwork
{
public:
	//constructor!

	size_t vertices; //vertices in the network

	//OD pairs

	//OD demands

	//paths per OD pair

	//
	vector<vector<double>> standardCapacities;
	vector<vector<double>> freeFlowTimes;

	double TravelTimeRoad(double capacity, double freeFlow, double actualFlow) {//returns travel time on an arc, given capacity, freeFlow, actualFlow
		return(freeFlow * (1 + 0.15 * pow(actualFlow / freeFlow, 4)));
	}
};

