#include "MaintenanceActivities.h"


#include "ScheduleCheckFunctions.h"

using namespace std;

bool bruteForceSchedule(MaintenanceActivities &Maint, size_t schedule, size_t runOutTime, vector<vector<size_t>>& binarySchedule) {
	//returns if schedule from wholenumber is feasible wrt ends before timeHorizon + returns the binarySchedule
	bool ifFeasible = true;
	//set binary schedule
	vector<size_t> startTimes(Maint.M, 0);//start at time 1, 0 is the equilibrium time!
	wholeScheduleToMaintenance(Maint.T - runOutTime, Maint.M, schedule, startTimes);

	for (size_t m = 0; m < Maint.M; ++m) {
		if (startTimes[m] + Maint.duration[m] > (Maint.T - runOutTime) || startTimes[m] == 0) {
			ifFeasible = false;
			break;
		}
	}
	if (ifFeasible) {
		for (size_t t = 1; t < Maint.T; ++t)
			for (size_t m = 0; m < Maint.M; ++m) {
				if (t >= startTimes[m] && t < startTimes[m] + Maint.duration[m]) {
					binarySchedule[t][m] = 1;
				}
				else {
					binarySchedule[t][m] = 0;
				}
			}
		//print2Dim(Sched.binarySchedule);

		//update scheduled capacities using binary schedule
		//binaryToCapacities(Maint.T, Maint.M, Sched.binarySchedule, Maint.locationSets, Net.standardCapacities, Sched.scheduledCapacities);

		//update route availability, based on binarySchedule, return false if not a feasibleSchedule
		//ifFeasible = adjustAvailableRoutes(Maint.T, Maint.M, Net.numberODpairs, Net.numberODpaths, Net.ODpaths, Sched.binarySchedule, Maint.locationSets, Maint.interruptedRoutes, Sched.availableRoutes, Sched.numAvailableRoutes);
	}

	return ifFeasible;
}