#include "ScheduleCheckFunctions.h"
#include "generalPrintingFunctions.h"

#include <vector>
#include <numeric> //for iota
#include <cstddef> //necessery for size_t!!

using namespace std;

bool adjustAvailableRoutes(size_t T, size_t M, size_t ODpairs, vector<size_t> &numberODpaths, vector<vector<vector<size_t>>> &ODpaths, const std::vector< std::vector<size_t>>& binarySchedule, const std::vector<std::vector<size_t>>& locations, vector<vector<vector<size_t>>> &maintenanceToRouteInterruption, std::vector<std::vector<std::vector<size_t>>>& availableRoutes, std::vector<std::vector<size_t>>& numAvailableRoutes){
	//in the final 2 arguments changes the available routes per period and the number of available routes per period.
	//maintenanceToRouteInterruption is a binary: 1 if route IS unavailable.
	
	bool feasibleSchedule = true;
	
	vector<size_t> availables;
	vector<size_t>::iterator locRoute;

	for(size_t t = 0; t < T; ++t)
		for (size_t od = 0; od < ODpairs; ++od) {
			availables = vector<size_t>(numberODpaths[od], 1);
			iota(availables.begin(), availables.end(), 0);//fill with numbers 0, 1, ..., r

			//check in binary schedule when each is implemented
			//use (binary) vector of which route is unavailable bc of this maintenance
			//remove if unavailable

			//?? WRONG!! since it is added if it is not in 1 maintenance, but could be in another maintenance...
			for (size_t m = 0; m < M; ++m) {
				if (binarySchedule[t][m] > 0) {
					for (size_t a = 0; a < numberODpaths[od]; ++a) {
						if (maintenanceToRouteInterruption[m][od][a] == 1) {//if it is interrupted
							//remove from availables
							locRoute = find(availables.begin(), availables.end(), a);
							if (locRoute != availables.end()) {
								availables.erase(locRoute);
							}
						}
					}
				}
			}

			availableRoutes[t][od] = availables;
			numAvailableRoutes[t][od] = availableRoutes[t][od].size();
			if (availables.size() == 0) {
				feasibleSchedule = false;
				break;
			}
		}
		

	return feasibleSchedule;
}