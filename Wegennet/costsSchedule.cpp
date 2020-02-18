#include "costCalculationFunctions.h"
#include "RoadNetwork.h"

#include <vector>

using namespace std;

double costsSchedule(RoadNetwork &Network, size_t T, vector<vector<vector<double>>>& scheduledCapacities, vector<vector<vector<double>>>& arcFlows) {
	
	double travelTime = totalTravelTime(Network, T, scheduledCapacities, arcFlows);

	

	return travelTime;
}