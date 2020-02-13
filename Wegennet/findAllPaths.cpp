#include "DepthFirstSearchFunctions.h"
#include "RoadNetwork.h"
#include "generalPrintingFunctions.h"
#include <vector>

using namespace std;

void findAllPaths(size_t od, RoadNetwork &Network) {

	//start from initial
	vector<bool> visited(Network.vertices, false);
	vector<size_t> path(Network.vertices, 0);
	vector<vector<size_t>> allPaths(0, vector<size_t>(0, 0));
	dfsRecursive(Network.ODpairs[2 * od], Network.ODpairs[2 * od + 1],  Network.vertices, Network.standardCapacities, allPaths, 0, visited, path);
	
	//find number of routes found:
	Network.ODpaths.push_back(allPaths);
	Network.numberODpaths.push_back(allPaths.size());

	cout << "saved check ---" <<Network.numberODpaths[od] <<"----\n";
	print2Dim(Network.ODpaths[od]);
	cout << "-------\n";
}