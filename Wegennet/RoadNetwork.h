
#pragma once
#include <vector>
#include <string>
#include <fstream>

using namespace std;
class RoadNetwork
{
private:
	void readPaths(ifstream& read) {
		//for now assume we have the paths in the file!

		vector<vector<vector<size_t>>> ODroutes;
		vector<vector<size_t>> routesPerOD;
		vector<size_t> route;
		size_t next = 0;
		for (size_t o = 0; o < numberODpairs; ++o) {
			routesPerOD.clear();
			for (size_t p = 0; p < numberODpaths[o]; ++p) {
				route.clear();
				next = ODpairs[2 * o];
				while (next != ODpairs[2 * o + 1]) {
					read >> next;
					route.push_back(next);
				}
				routesPerOD.push_back(route);
			}
			ODroutes.push_back(routesPerOD);
		}

		ODpaths = ODroutes;
		for (size_t n = 0; n < numberODpairs; ++n) {
			numberODpaths[n] = ODpaths[n].size();
		}
	}

public:
	//constructor!
	RoadNetwork(string inputDoc);

	size_t vertices; //vertices in the network

	//OD pairs
	size_t numberODpairs;
	vector<size_t> ODpairs;

	//OD demands
	vector<double> ODdemands; //demand per OD pair

	//paths per OD pair
	vector<size_t> numberODpaths; // how many paths per OD pair (also size of the next..?)
	vector<vector<vector<size_t>>> ODpaths;//which OD, which path, order of vertices
	//
	vector<vector<double>> standardCapacities;
	vector<vector<double>> freeFlowTimes;

	double TravelTimeRoad(double capacity, double freeFlow, double actualFlow) {//returns travel time on an arc, given capacity, freeFlow, actualFlow
		return(freeFlow * (1 + 0.15 * pow(actualFlow / freeFlow, 4)));
	}

	
};

