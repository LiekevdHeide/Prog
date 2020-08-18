#include "VNS.h"

#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"
#include "costCalculationFunctions.h"

#include <random>
#include <vector>
#include <cstddef> //necessery for size_t!!

using namespace std;
void shakeTimeNBH(MaintenanceActivities &Maintenance, ScheduleAndFlows &Sched) {//adjusts Schedule.startTimes, but not Schedule.binary
	//selects random maintenance activity + random startTime (between 1 and, T-runout-duration!)
	random_device rd;
	mt19937 randomGenerator(rd());
	uniform_int_distribution<std::mt19937::result_type> chooseM(0, Maintenance.M - 1);

	size_t mToShift = chooseM(randomGenerator);
	//cout << "shake:" << mToShift << ' ';
	uniform_int_distribution<mt19937::result_type> chooseT(1, Maintenance.T - Maintenance.runOutPeriod - Maintenance.duration[mToShift]);
	size_t shiftT = chooseT(randomGenerator);
	

	Sched.startTimes[mToShift] = shiftT;
	shiftToOne(Maintenance.M, Sched.startTimes);

	return;
}