#include "ConvexCombinations.h"
#include "RoadNetwork.h"

#include <vector>
#include <iostream>
using namespace std;
double calculateConvergenceTest(vector<vector<double>> &oldArcFlows, vector<vector<double>> &newArcFlows, RoadNetwork &Roads, vector<vector<double>> actualCapacities, vector<size_t> numberAvailablePaths, vector<vector<size_t>> availablePaths) {
	
	double max = 0.0;
	double test = 0.0;
	size_t shortestOld = 0;
	size_t shortestNew = 0;
	for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			//find shortest paths (implicitly also calculates the corr times...)
			shortestOld = findShortestPath(Roads, od, oldArcFlows, actualCapacities, numberAvailablePaths[od], availablePaths[od]);
			shortestNew = findShortestPath(Roads, od, newArcFlows, actualCapacities, numberAvailablePaths[od], availablePaths[od]);

			//|min path travel time at nth iteration - min path travel time at n-1th iteration| / min path travel time iteration n 
			test = abs(Roads.pathTravelTime(Roads.ODpaths[od][shortestNew], newArcFlows) - Roads.pathTravelTime( Roads.ODpaths[od][shortestOld], oldArcFlows) );
			
			//cout << "test:" << Roads.pathTravelTime(Roads.ODpaths[od][shortestNew], newArcFlows) << ' ' << Roads.pathTravelTime(Roads.ODpaths[od][shortestOld], oldArcFlows) << '\n';
			
			//can divide by first part?
			if (test > max) {
				max = test;
			}
	}
	return max;
}