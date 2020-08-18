#include "PSAP.h"
#include "ScheduleAndFlows.h"

#include <iostream>
#include <cstddef> //necessery for size_t!!

using namespace std;

double determineFlowClosingPaths(ScheduleAndFlows &Schedule, size_t t, size_t od) {
	//finds recurrent flow (i.e. Schedule.pathFlow) at closing path and outputs it. Does not remove it.
	
	double flowAtClosingPath = 0.0;

	//find flows on paths at t, which are unavailable at t+1!
	for (size_t p = 0; p < Schedule.numAvailableRoutes[t][od]; ++p) {
		if (find(Schedule.availableRoutes[t + 1][od].begin(), Schedule.availableRoutes[t + 1][od].end(), Schedule.availableRoutes[t][od][p]) == Schedule.availableRoutes[t + 1][od].end()) {
			flowAtClosingPath += Schedule.pathFlow[t][od][Schedule.availableRoutes[t][od][p]];
			//cout << "CLp:" << Schedule.availableRoutes[t][od][p] << ' ';
		}
	}
	
	//cout << "ClPFlow:" << flowAtClosingPath << '@';

	return flowAtClosingPath;
}