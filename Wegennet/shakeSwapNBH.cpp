#include "VNS.h"

#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"
#include "costCalculationFunctions.h"

#include <random>
#include <vector>

using namespace std;
void shakeSwapNBH(MaintenanceActivities Maintenance, ScheduleAndFlows Sched) {//adjusts Schedule.startTimes, but not Schedule.binary
	//selects random maintenance activitys and mirror/swaps them: POSSIBLY THE SAME MAINTENANCE SELECTED TWICE

	random_device rd;
	mt19937 randomGenerator(rd());
	uniform_int_distribution<std::mt19937::result_type> chooseM(0, Maintenance.M - 1);

	size_t main1 = chooseM(randomGenerator);
	size_t main2 = chooseM(randomGenerator);
	//cout << "shake:" << main1 << ' ' << main2 << "oldTimes: " << Sched.startTimes[main1] << ' ' << Sched.startTimes[main2] << " newTimes: ";

	size_t minStart = min(Sched.startTimes[main1], Sched.startTimes[main2]);
	size_t maxEnd = max(Sched.startTimes[main1] + Maintenance.duration[main1], Sched.startTimes[main2] + Maintenance.duration[main2]);

	Sched.startTimes[main1] = minStart + maxEnd - Sched.startTimes[main1] - Maintenance.duration[main1];
	Sched.startTimes[main2] = minStart + maxEnd - Sched.startTimes[main2] - Maintenance.duration[main2];

	shiftToOne(Maintenance.M, Sched.startTimes);

	return;
}