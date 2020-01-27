#include "ScheduleCheckFunctions.h"

#include <vector>

using namespace std;

void adjustAvailableRoutes(size_t T, size_t M, size_t ODpairs, vector<size_t> &numberODpaths, vector<vector<vector<size_t>>> &ODpaths, const std::vector< std::vector<size_t>>& binarySchedule, const std::vector<std::vector<size_t>>& locations, vector<vector<vector<size_t>>> &maintenanceToRouteInterruption, std::vector<std::vector<std::vector<size_t>>>& availableRoutes, std::vector<std::vector<size_t>>& numAvailableRoutes){
	//in the final 2 arguments changes the available routes per period and the number of available routes per period.
	//maintenanceToRouteInterruption is a binary: 1 if route IS unavailable.
	vector<size_t> availables;
	for(size_t t = 0; t < T; ++t)
		for (size_t od = 0; od < ODpairs; ++od) {
			availables = vector<size_t>(numberODpaths[od], 1);

			//check in binary schedule when each is implemented
			//use vector of which route is unavailable bc of this maintenance

			//WRONG!! since it is added if it is not in 1 maintenance, but could be in another maintenance...
			for (size_t m = 0; m < M; ++m) {
				if (binarySchedule[t][m] > 0) {
					for (size_t a = 0; a < numberODpaths[od]; ++a) {// maintenanceToRouteInterruption[m][od].size()
						if (maintenanceToRouteInterruption[m][od][a] == 0) {//if it is not interrupted
							availables[a] = a;//set it to a recognisable number.
						}
						else {//it is interrupted
							availables[a] = numberODpaths[od];//higher than possible
						}


					}
				}
			}
			for (size_t r = 0; r < numberODpaths[od]; ++r) {
				if (availables[r] == numberODpaths[od]) {//if pathnumber > than possible:

					//NOW SKIPS OVER THINGS SINCE YOU ERASE IT!
					availables.erase(availables.begin() + r);
				}
			}


			availableRoutes[t][od] = availables;
			numAvailableRoutes[t][od] = availableRoutes[t][od].size();
		}


	return;
}