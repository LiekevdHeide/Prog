#include "ConvexCombinations.h"
#include "RoadNetwork.h"

#include <vector>

using namespace std;
size_t findShortestPath(RoadNetwork Roads, size_t od, vector<vector<double>> arcFlows) { //ASSUMES T=0!!!
	//uses the pathtraveltime according to what is saved in Roads, with the flow as in pathFlows.
	size_t shortestPath = 0;
	double pathLength = 0.0;

	//initialize to first path, otherwise need a value to start at
	double shortestPathLength{ Roads.pathTravelTime(Roads.ODpaths[od][0], arcFlows) };//err

	//cout << shortestPathLength << ' ';
	for (size_t p = 1; p < Roads.numberODpaths[od]; ++p) {
		//calculate path length
		pathLength = Roads.pathTravelTime(Roads.ODpaths[od][p], arcFlows);
		//cout << pathLength << ' ';
		//if < shortest: update
		if (pathLength < shortestPathLength) {
			shortestPathLength = pathLength;
			shortestPath = p;
		}
	}
	return shortestPath;
}