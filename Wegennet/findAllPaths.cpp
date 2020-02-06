#include "DepthFirstSearchFunctions.h"
#include "RoadNetwork.h"

#include <vector>

using namespace std;

void findAllPaths(size_t origin, size_t destination, RoadNetwork Network) {

	vector<size_t> path;

	//start from initial
	vector<bool> visited(Network.vertices, false);

	dfsRecursive(origin, visited, Network.vertices, Network.standardCapacities);

}