#include "RoadNetwork.h"
#include "DepthFirstSearchFunctions.h"

#include <vector>

using namespace std;

vector<size_t> findShortestAlternativeTourist(RoadNetwork& Network, const vector<vector<double>> &currentCapacities, size_t origin, size_t destination) {
	vector<bool> visited(Network.vertices, false);
	vector<size_t> path(Network.vertices, 0);
	vector<vector<size_t>> allPaths(0, vector<size_t>(0, 0));
	//find all routes between origin and destination
	dfsRecursive(origin, destination, Network.vertices, currentCapacities, allPaths, 0, visited, path);

	//find shortest:
	double travelTime = 0.0;
	vector<vector<double>> emptyFlows(Network.vertices, vector<double>(Network.vertices, 0.0));

	double shortestTravelTime = Network.pathTravelTime(allPaths[0], emptyFlows);
	//const vector<size_t>& path, const vector<vector<double>>& arcFlows, const vector<vector<double>> &actualCapacities
	size_t shortestPath = 0;
	for (size_t p = 1; p < allPaths.size(); ++p) {
		travelTime = Network.pathTravelTime(allPaths[p], emptyFlows);
		if (travelTime < shortestTravelTime) {
			shortestTravelTime = travelTime;
			shortestPath = p;
		}
	}

	for (size_t r = 0; r < allPaths[shortestPath].size(); ++r) {
		cout << allPaths[shortestPath][r] << ' ';
	}
	cout << shortestTravelTime;
	cout << '\n';
	return allPaths[shortestPath];
}
