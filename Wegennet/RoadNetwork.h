
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
class RoadNetwork
{
private:
	//created alternative for this: (depthFirstSearch.cpp)
	/*void readPaths(ifstream& read) {
		//for now assume we have the paths in the file!

		vector<vector<vector<size_t>>> ODroutes;
		vector<vector<size_t>> routesPerOD;
		vector<size_t> route;
		size_t next = 0;
		cout << "routes:\n";
		for (size_t o = 0; o < numberODpairs; ++o) {
			routesPerOD.clear();
			for (size_t p = 0; p < numberODpaths[o]; ++p) {
				route.clear();
				next = ODpairs[2 * o];
				while (next != ODpairs[2 * o + 1]) {
					read >> next;
					cout << next << ' ';
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
	}*/

public:
	//constructor!
	RoadNetwork(string inputDoc);

	size_t vertices; //vertices in the network
	double touristPercentage;
	double muCostUninformed;

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

	double travelTimeRoad(double capacity, double freeFlow, double actualFlow) {//returns travel time on an arc, given capacity, freeFlow, actualFlow
		return(freeFlow * (1 + 0.15 * pow(actualFlow / capacity, 4)));//capacity??
	}

	double pathTravelTime(const vector<size_t> &path, const vector<vector<double>> &arcFlows) {
		//only uses the freeflow + capacities in this class. Flow is input.
		double travelTime = 0.0;
		for (size_t a = 0; a < path.size() - 1; ++a) {
			travelTime += travelTimeRoad(standardCapacities[path[a]][path[a + 1]], freeFlowTimes[path[a]][path[a + 1]], arcFlows[path[a]][path[a + 1]]);
		}
		return travelTime;
	}

	double pathTravelTime(const vector<size_t>& path, const vector<vector<double>>& arcFlows, const vector<vector<double>> &actualCapacities) {
		//only uses the freeflow + capacities in this class. Flow is input.
		double travelTime = 0.0;
		for (size_t a = 0; a < path.size() - 1; ++a) {
			size_t b0 = path[a];
			size_t b1 = path[ a + 1];
			//travelTime += travelTimeRoad(actualCapacities[path[a]][path[a + 1]], freeFlowTimes[path[a]][path[a + 1]], arcFlows[path[a]][path[a + 1]]);
			travelTime += travelTimeRoad(actualCapacities[b0][b1], freeFlowTimes[b0][b1], arcFlows[b0][b1]);
		}
		return travelTime;
	}


	
};

