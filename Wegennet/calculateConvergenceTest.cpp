#include "ConvexCombinations.h"
#include "RoadNetwork.h"

#include <vector>
#include <iostream>
using namespace std;
double calculateConvergenceTest(vector<vector<double>> &oldPathFlows, vector<vector<double>> &newPathFlows, RoadNetwork &Roads) {
	
	double max = 0.0;
	double test = 0.0;
	size_t shortestOld = 0;
	size_t shortestNew = 0;
	for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			//find shortest paths (implicitly also calculates the corr times...)
			shortestOld = findShortestPath(Roads, od, oldPathFlows[od]);//err
			shortestNew = findShortestPath(Roads, od, newPathFlows[od]);

			//|min path travel time at nth iteration - min path travel time at n-1th iteration| / min path travel time iteration n 
			test = abs(Roads.pathTravelTime(Roads.ODpaths[od][shortestNew], newPathFlows[od][shortestNew]) - Roads.pathTravelTime( Roads.ODpaths[od][shortestOld], oldPathFlows[od][shortestOld]) );
			cout << "test:" << Roads.pathTravelTime(Roads.ODpaths[od][shortestNew], newPathFlows[od][shortestNew]) << ' ' << Roads.pathTravelTime(Roads.ODpaths[od][shortestOld], oldPathFlows[od][shortestOld]) << '\n';
			
			//can divide by first part?
			if (test > max) {
				max = test;
			}
	}
	return max;
}