#include "PSAP.h"
#include "RoadNetwork.h"

using namespace std;

void findTouristFlows(vector<vector<double>> &scheduledCapacities, RoadNetwork &Roads, vector<vector<double>> &touristFlows) { //at one time!

	//for implemented maintenance plan
	vector<vector<double>> zeroFlows = vector<vector<double>>(Roads.vertices, vector<double>(Roads.vertices, 0.0));
	double shortestRouteTime;
	double thisRouteTime;
	size_t shortestRoute = 0;

	//set all flows to 0
	touristFlows = zeroFlows;
	for (size_t od = 0; od < Roads.numberODpairs; ++od) {
		//set to 0 flows everywhere

		//check if a route is impossible bc maintenance.
		for (size_t p = 0; p < Roads.numberODpaths[od]; ++p) {
			//check if maintenance planned on this route.


		//find shortest route if no flows
			shortestRouteTime = Roads.pathTravelTime(Roads.ODpaths[od][0], zeroFlows, scheduledCapacities);
			shortestRoute = 0;
			cout << shortestRouteTime << ":" << shortestRoute << ' ';
			for (size_t r = 1; r < Roads.numberODpaths[od]; ++r) {
				if (r != p) {
					//find shortest no flow route as suggested alternative
					thisRouteTime = Roads.pathTravelTime(Roads.ODpaths[od][r], zeroFlows, scheduledCapacities);
					if (thisRouteTime < shortestRouteTime) {
						shortestRouteTime = thisRouteTime;
						shortestRoute = r;
						cout << shortestRouteTime << ':' << shortestRoute << ' ';
					}
				}
			}

			//add tourist flows to the alternative routes: ALL ODs ADDED
			for (size_t a = 0; a < Roads.ODpaths[od][shortestRoute].size() - 1; ++a) {
				touristFlows[Roads.ODpaths[od][shortestRoute][a]][Roads.ODpaths[od][shortestRoute][a + 1]] += Roads.ODdemands[od] * Roads.touristPercentage;
			}
		}
	}

	return;
}