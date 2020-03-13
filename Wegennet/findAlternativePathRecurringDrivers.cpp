#include "PSAP.h"

#include <vector>
#include <iostream>

using namespace std;

size_t findAlternativePathRecurringDrivers(ScheduleAndFlows &Schedule, size_t t, size_t od, vector<vector<double>> &pathTimes) {
	//Find index (of available paths) of (expected) shortest path (using pathTimes) for recurring at t + 1 and return it

	size_t indexShortest = 0;
	for (size_t p = 0; p < Schedule.numAvailableRoutes[t + 1][od]; ++p) {
		//find shortest path at t (which is available at t+1) and add flowAtClosingPath
		if (pathTimes[od][Schedule.availableRoutes[t + 1][od][p]] < pathTimes[od][Schedule.availableRoutes[t + 1][od][indexShortest]]) {
			indexShortest = p;
		}
	}

	//cout << Schedule.availableRoutes[t + 1][od][indexShortest] << ' ';

	return indexShortest;
}