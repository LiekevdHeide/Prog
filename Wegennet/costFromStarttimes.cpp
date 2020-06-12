#include "costCalculationFunctions.h"


#include "RoadNetwork.h"
#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"

#include <vector>

using namespace std;

double costFromStarttimes(RoadNetwork& Network, MaintenanceActivities& Maintenance, ScheduleAndFlows& Schedule, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, size_t numSmallStep, double biGC) {

	for (size_t m = 0; m < Maintenance.M; ++m) {
		for (size_t t = 0; t < Maintenance.T; ++t) {
			if (t < Schedule.startTimes[m] || t >= Schedule.startTimes[m] + Maintenance.duration[m]) {
				Schedule.binarySchedule[t][m] = 0;
			}
			else {
				Schedule.binarySchedule[t][m] = 1;
			}
		}
	}

	return costFromSchedule(Network, Maintenance, Schedule, touristAltPerwholeState, numSmallStep, biGC);
}