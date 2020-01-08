
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"
#include "PSAP.h"

#include "generalPrintingFunctions.h"

#include <algorithm>
#include <vector>
using namespace std;

void proportionalSwitch(size_t T, RoadNetwork &Network, ScheduleAndFlows &Schedule) { //assumes at 0 no maintenance!
	
	vector<vector<double>> pathTimes(Network.numberODpairs, vector<double>());
	for(size_t od = 0; od < Network.numberODpairs; ++ od){
			pathTimes[od] = vector<double>(Network.numberODpaths[od]);
		}
	cout << Schedule.pathFlow[0][0][0] << ' ';
	for (size_t t = 0; t < T - 1; ++t) {
		//calculate all arc times given current flows!
		//cout << "Time: " << t << "\n";
		for (size_t od = 0; od < Network.numberODpairs; ++od) {
			cout << "Pathtimes: ";
			for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
				pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][p], Schedule.arcFlowAll[t], Schedule.scheduledCapacities[t]);//patharcs, flows, caps)
				cout << pathTimes[od][p] << ' ';
			}

			for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
				Schedule.pathFlow[t + 1][od][p] = Schedule.pathFlow[t][od][p];
				for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
					if (r != p) {
						Schedule.pathFlow[t + 1][od][p] += 0.01 * Schedule.pathFlow[t][od][r] * max(pathTimes[od][r] - pathTimes[od][p], 0.0) - Schedule.pathFlow[t][od][p] * max(pathTimes[od][p] - pathTimes[od][r], 0.0);
					}
				}
			}

		}

		Schedule.updateArcFlowAll(t + 1, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);
		//cout << "\nUpdated paths t" << t + 1 << '\n';
		//print2Dim(Schedule.pathFlow[t + 1], Network.numberODpairs);
		//print2Dim(Schedule.arcFlowAll[t + 1], Network.vertices);
		cout << "FLOW 0 " << Schedule.pathFlow[t + 1][0][0] << ' ';

		//for each OD pair
			//for all paths
		//determine path time given all flows (using calculated time)

		//set flow for t+1

	}


	return;
}