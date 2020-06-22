#include "VNS.h"

#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"

#include <random>
#include <vector>

using namespace std;
void shakeTimeNBH(MaintenanceActivities &Maintenance, ScheduleAndFlows &Sched, size_t runoutT) {//adjusts Schedule.startTimes, but not Schedule.binary
	//selects random maintenance activity + random startTime (between 1 and, T-runout-duration!)
	random_device rd;
	mt19937 randomGenerator(rd());
	uniform_int_distribution<std::mt19937::result_type> chooseM(0, Maintenance.M - 1);

	size_t mToShift = chooseM(randomGenerator);
	cout << "shake:" << mToShift << ' ';
	uniform_int_distribution<std::mt19937::result_type> chooseT(1, Maintenance.T - runoutT - Maintenance.duration[mToShift]);
	size_t shiftT = chooseT(randomGenerator);
	cout << shiftT << '\n';
	

	Sched.startTimes[mToShift] = shiftT;

	return;
}