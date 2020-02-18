#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "generalPrintingFunctions.h"

#include "ScheduleCheckFunctions.h"

using namespace std;

bool bruteForceSchedule(ScheduleAndFlows &Sched, MaintenanceActivities &Maint, RoadNetwork &Net, size_t schedule) {

	//set binary schedule
	vector<size_t> startTimes(Maint.M, 0);
	wholeScheduleToMaintenance(Maint.T, Maint.M, schedule, startTimes);
	for(size_t t = 1; t < Maint.T; ++t)
		for (size_t m = 0; m < Maint.M; ++m) {
			if (t >= startTimes[m] && t < startTimes[m] + Maint.duration[m]) {
				Sched.binarySchedule[t][m] = 1;
			}
			else {
				Sched.binarySchedule[t][m] = 0;
			}
		}
	//print2Dim(Sched.binarySchedule);
	
	//update scheduled capacities using binary schedule
	binaryToCapacities(Maint.T, Maint.M, Sched.binarySchedule, Maint.locationSets, Net.standardCapacities, Sched.scheduledCapacities);

	//update route availability, based on binarySchedule, return false if not a feasibleSchedule
	bool ifFeasible = adjustAvailableRoutes(Maint.T, Maint.M, Net.numberODpairs, Net.numberODpaths, Net.ODpaths, Sched.binarySchedule, Maint.locationSets, Maint.interruptedRoutes, Sched.availableRoutes, Sched.numAvailableRoutes);

	return ifFeasible;
}