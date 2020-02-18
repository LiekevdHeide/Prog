#include "PSAP.h"
#include "RoadNetwork.h"
#include "DepthFirstSearchFunctions.h"

#include <vector>
#include <iostream>

using namespace std;

void findAlternativeArcFlowsTourists(RoadNetwork &Network, const vector<vector<double>> &currentCapacities, vector<vector<double>> &arcFlows) {
	//use maintenance locations to determine the blocked arcs
	//then determine the alternative flows
	double divertedFlow = 0.0;
	vector<size_t> path;

	for (size_t v = 0; v < Network.vertices; ++v) {
		for (size_t w = 0; w < Network.vertices; ++w) {
			if (currentCapacities[v][w] < 1 && arcFlows[v][w] > 0) { //FOR NOW < 1!! (check what happens if capacity is set to 0)
				cout << v << '-' << w << ' ' << arcFlows[v][w] << ' ';
				divertedFlow = arcFlows[v][w];
				arcFlows[v][w] = 0;

				//use some sort of DFS to get alternative path.. from v -> w
				path = findShortestAlternativeTourist(Network, currentCapacities, v, w);
				if (path.size() > 0) {//if there is an alternative: add flow to it
					for (size_t p = 0; p < path.size() - 1; ++p) {
						arcFlows[path[p]][path[p + 1]] += divertedFlow;
					}
				}
				//if no alternative path: flow will be adjusted at an earlier point

			}
		}
	}

	return;
}