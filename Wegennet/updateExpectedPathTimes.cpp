#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"
#include "PSAP.h"

#include <vector>
#include <iostream>

using namespace std;

void updateExpectedPathTimes(RoadNetwork &Network, ScheduleAndFlows &Schedule, size_t t, size_t od, vector<vector<double>> &pathTimes) {

	//set all pathTimes to free flow time when no maintenance is scheduled:
	for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
		pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][p], vector<vector<double>>(Network.vertices, vector<double>(Network.vertices, 0.0)), Network.standardCapacities);
	}

	//change pathtimes for available routes: based on both types of drivers.
	for (size_t p = 0; p < Schedule.numAvailableRoutes[t][od]; ++p) {
		pathTimes[od][Schedule.availableRoutes[t][od][p]] = Network.pathTravelTime(Network.ODpaths[od][Schedule.availableRoutes[t][od][p]], Schedule.arcFlowAll[t], Schedule.scheduledCapacities[t]);//patharcs (for available route), flows, caps)
		//cout << Schedule.availableRoutes[t][od][p] << ':' << pathTimes[od][p] << ' ';
	}

	return;
}