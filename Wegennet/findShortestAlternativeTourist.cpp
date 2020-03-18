#include "RoadNetwork.h"
#include "DepthFirstSearchFunctions.h"

#include <vector>

using namespace std;

vector<size_t> findShortestAlternativeTourist(RoadNetwork& Network, const vector<vector<double>> &currentCapacities, size_t origin, size_t destination) {
	vector<size_t> returnShortestPath;
	
	vector<bool> visited(Network.vertices, false);
	vector<size_t> path(Network.vertices, 0);
	vector<vector<size_t>> allPaths(0, vector<size_t>(0, 0));

	//find all routes between origin and destination
	dfsRecursive(origin, destination, Network.vertices, currentCapacities, allPaths, 0, visited, path);

	size_t shortestPath = 0;
	if (allPaths.size() > 0) {
		//find shortest:
		double travelTime = 0.0;
		vector<vector<double>> emptyFlows(Network.vertices, vector<double>(Network.vertices, 0.0));

		double shortestTravelTime = Network.pathTravelTime(allPaths[0], emptyFlows);
		
		for (size_t p = 1; p < allPaths.size(); ++p) {
			travelTime = Network.pathTravelTime(allPaths[p], emptyFlows);
			if (travelTime < shortestTravelTime) {
				shortestTravelTime = travelTime;
				shortestPath = p;
			}
		}

		returnShortestPath = allPaths[shortestPath];
	}
	
	//if no paths from origin to destination return an empty path

	return returnShortestPath;
}
