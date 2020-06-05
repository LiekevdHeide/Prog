#include "VNS.h"

#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"

#include "generalPrintingFunctions.h"

#include <random>
#include <vector>

using namespace std;
void shakeTimeNBH(MaintenanceActivities &Maintenance, ScheduleAndFlows &Sched, size_t runoutT) {
	random_device rd;
	mt19937 randomGenerator(rd());
	uniform_int_distribution<std::mt19937::result_type> chooseM(0, Maintenance.M - 1);

	size_t mToShift = chooseM(randomGenerator);
	cout << "shake:" << mToShift << ' ';
	uniform_int_distribution<std::mt19937::result_type> chooseT(1, Maintenance.T - runoutT - Maintenance.duration[mToShift]);
	size_t shiftT = chooseT(randomGenerator);
	cout << shiftT << '\n';
	//shift them in binarySchedule:
	for (size_t t = 1; t < Maintenance.T; ++t) {
		if (t < shiftT || t >= shiftT + Maintenance.duration[mToShift]) {
			Sched.binarySchedule[t][mToShift] = 0;
		}
		else {
			Sched.binarySchedule[t][mToShift] = 1;
		}
	}

	print2Dim(Sched.binarySchedule);

	return;
}