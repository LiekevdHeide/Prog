#include "costCalculationFunctions.h"
#include "RoadNetwork.h"
#include "printingResults.h"

#include <vector>
#include <cstddef> //necessery for size_t!!

using namespace std;
void printCostsPerTime(ofstream &write, RoadNetwork& Network, size_t T, vector<vector<vector<double>>>& scheduledCapacities, vector<vector<vector<double>>>& arcFlows) {
	double travelTime = 0.0;
	for (size_t t = 0; t < T; ++t) {
		for (size_t v = 0; v < Network.vertices; ++v) {
			for (size_t w = 0; w < Network.vertices; ++w) {
				if (arcFlows[t][v][w] > 0) {
					travelTime += arcFlows[t][v][w] * Network.travelTimeRoad(scheduledCapacities[t][v][w], Network.freeFlowTimes[v][w], arcFlows[t][v][w]);
				}
			}
		}
		write << travelTime << ' ';
		travelTime = 0;
	}
	//cout << travelTime << ' ';
	return;
}