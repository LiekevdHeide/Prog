#include "ScheduleCheckFunctions.h"
#include "generalPrintingFunctions.h"

#include <algorithm>

//OLD! (before IP)

void touristAlternative(size_t OD, vector<size_t> numRoutes, vector<vector<vector<size_t>>> &odRoutes, size_t M, vector<vector<size_t>> &locations) {
	//Find alternative tourist flows for all combinations of maintenances

	//find which paths are interrupted
	vector<vector<vector<size_t>>> interruptedRoutes(M, vector<vector<size_t>>(OD, vector<size_t>()));
	for(size_t m = 0; m < M; ++m)
		for(size_t od = 0; od < OD; ++od)
			for (size_t r = 0; r < numRoutes[od]; ++r) {
				interruptedRoutes[m][od] = vector<size_t>(numRoutes[od], 0);
			}
	findInterruptedRoutes(M, locations, OD, numRoutes, odRoutes, interruptedRoutes);// input: locations is , returns interruptedRoutes (binary).


	vector<size_t> schedule(M, 0);
	vector<size_t> unavailableArcs;
	//for each binary / wholeschedule:
	cout << "schedules:\n";
	for (size_t s = 0; s < pow(2, M); ++s) {
		//use wholeScheduleToMaintenance to get the binary for it
		wholeScheduleToMaintenance(2, M, s, schedule);
		for (size_t a = 0; a < schedule.size(); ++a) {
			cout << schedule[a] << ' ';
		}
		cout << '\n';

		//for each od pair, path: add flow 



		unavailableArcs.clear();
	}
	



}