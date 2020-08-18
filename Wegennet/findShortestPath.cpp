#include "ConvexCombinations.h"
#include "RoadNetwork.h"

#include <vector>
#include <cstddef> //necessery for size_t!!

using namespace std;
size_t findShortestPath(RoadNetwork Roads, size_t od, vector<vector<double>> arcFlows, vector<vector<double>> actualCapacities, size_t numberAvailablePaths, vector<size_t> availablePaths, double &lengthReturn) { //No longer assumes t = 0?
	//uses the pathtraveltime according to what is saved in Roads, with the flow as in pathFlows.
	size_t shortestPath = 0;
	double pathLength = 0.0;
	double shortestPathLength = 0.0;
	//initialize to first path, otherwise need a value to start at
	
	for (size_t p = 0; p < numberAvailablePaths; ++p) {
		//calculate path length
		pathLength = Roads.pathTravelTime(Roads.ODpaths[od][availablePaths[p]], arcFlows, actualCapacities); //!!add current capacities!, change ODpaths to availablePaths
		//if < shortest: update
		if (pathLength < shortestPathLength || p == 0) {
			shortestPathLength = pathLength;
			shortestPath = availablePaths[p];
		}
	}
	lengthReturn = shortestPathLength;
	return shortestPath;
}