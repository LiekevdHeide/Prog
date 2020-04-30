#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"
#include "PSAP.h"

#include <vector>
#include <iostream>

using namespace std;

void updateExpectedPathTimes(RoadNetwork &Network, size_t od, size_t numAvailableRoutes, vector<size_t> &availableRoutes, vector<vector<double>> &scheduledCapacities, vector<vector<double>> &arcFlows, vector<double> &pathTimes) {
	//calculates path times at time t. Only for available paths: use pathTravelTime given capacity ONLY ADJUSTS AVAILABLE PATH TIMES

	//set all pathTimes to free flow time when no maintenance is scheduled:
	//for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
		//pathTimes[p] = Network.pathTravelTime(Network.ODpaths[od][p], vector<vector<double>>(Network.vertices, vector<double>(Network.vertices, 0.0)), Network.standardCapacities);
		//pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][p], Schedule.arcFlowAll[0]);
	//}

	//change pathtimes for available routes: based on both types of drivers.
	for (size_t p = 0; p < numAvailableRoutes; ++p) {
		pathTimes[availableRoutes[p]] = Network.pathTravelTime(Network.ODpaths[od][availableRoutes[p]], arcFlows, scheduledCapacities);//patharcs (for available route), flows, caps)
	}

	return;
}

/*
void updateExpectedPathTimes(RoadNetwork &Network, ScheduleAndFlows &Schedule, size_t t, size_t od, vector<vector<double>> &pathTimes) {
	//calculates path times at time t. For available paths: use pathTravelTime given capacity. For unavailable paths: free flow t

	//set all pathTimes to free flow time when no maintenance is scheduled:
	for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
		pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][p], vector<vector<double>>(Network.vertices, vector<double>(Network.vertices, 0.0)), Network.standardCapacities);
		//pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][p], Schedule.arcFlowAll[0]);
	}

	//change pathtimes for available routes: based on both types of drivers.
	for (size_t p = 0; p < Schedule.numAvailableRoutes[t][od]; ++p) {
		pathTimes[od][Schedule.availableRoutes[t][od][p]] = Network.pathTravelTime(Network.ODpaths[od][Schedule.availableRoutes[t][od][p]], Schedule.arcFlowAll[t], Schedule.scheduledCapacities[t]);//patharcs (for available route), flows, caps)
		//cout << Schedule.availableRoutes[t][od][p] << ':' << pathTimes[od][p] << ' ';
	}

	return;
}

*/