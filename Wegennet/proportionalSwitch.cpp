#include "ScheduleAndFlows.h"

#include <vector>
#include <algorithm>

using namespace std;

void proportionalSwitch(ScheduleAndFlows &Schedule, size_t t, size_t od, const vector<vector<double>> &pathTimes) {
	//implements the classic PSAP, returns new Schedule.pathFlows at t + 1, all else remains equal
	
	//change flows only for recurring drivers (so NOT tourists!), only consider changing from / to available paths at t+1
	for (size_t q = 0; q < Schedule.numAvailableRoutes[t + 1][od]; ++q) {
		//update pathflows based for all available at t + 1, based on t flows (FOR ALL AVAILABLE AT t and t + 1), only for non-tourists
		size_t p = Schedule.availableRoutes[t + 1][od][q];


		//if route not available at t: flow is 0.0 so OK
		Schedule.pathFlow[t + 1][od][p] = Schedule.pathFlow[t][od][p];//still excludes tourists.
		for (size_t g = 0; g < Schedule.numAvailableRoutes[t + 1][od]; ++g) { // if not available flow = 0
			size_t r = Schedule.availableRoutes[t + 1][od][g];
			
			if (r != p) {
				Schedule.pathFlow[t + 1][od][p] += 0.001 * (Schedule.pathFlow[t][od][r] * max(pathTimes[od][r] - pathTimes[od][p], 0.0) - Schedule.pathFlow[t][od][p] * max(pathTimes[od][p] - pathTimes[od][r], 0.0));

				//if no negative flow: NOT WORKING!
				//Schedule.pathFlow[t + 1][od][Schedule.availableRoutes[t + 1][od][p]] +=  min(Schedule.pathFlow[t][od][Schedule.availableRoutes[t + 1][od][r]], 0.01 * Schedule.pathFlow[t][od][Schedule.availableRoutes[t + 1][od][r]] * max(pathTimes[od][r] - pathTimes[od][p], 0.0)) - min(Schedule.pathFlow[t][od][Schedule.availableRoutes[t + 1][od][p]], 0.01 * Schedule.pathFlow[t][od][Schedule.availableRoutes[t + 1][od][p]] * max(pathTimes[od][p] - pathTimes[od][r], 0.0));


			}
		}
	}
	return;
}