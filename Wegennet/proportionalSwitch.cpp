
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"
#include "PSAP.h"

#include "generalPrintingFunctions.h"

#include <algorithm>
#include <vector>
using namespace std;

void proportionalSwitch(size_t T, RoadNetwork &Network, ScheduleAndFlows &Schedule) { //assumes at 0 no maintenance!
	
	vector<vector<double>> pathTimes(Network.numberODpairs, vector<double>());
	//vector<vector<size_t>> availableRoutes(Network.numberODpairs, vector<size_t>());//od, routes available?
	for(size_t od = 0; od < Network.numberODpairs; ++ od){
			pathTimes[od] = vector<double>(Network.numberODpaths[od]);
			//availableRoutes[od] = vector<size_t>(Network.numberODpaths[od]);//?
	}
	//vector<size_t> numAvailableRoutes(Network.numberODpairs, 0);
	
	
	//cout << Schedule.pathFlow[0][0][0] << ' ';

	vector<vector<double>> touristFlows(Network.vertices, vector<double>(Network.vertices, 0.0));

	//arcFlow: both drivers, pathflow: only recurrent drivers
	//arcflow input is ONLY THE TOURIST IN EQUILIBRIUM FLOW

	//subtract the tourists from the path flows:
	for(size_t od = 0; od < Network.numberODpairs; ++od)
		for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
			Schedule.pathFlow[0][od][r] *= (1 - Network.touristPercentage);
		}
	//allArcFlows + arcFlows still include tourists!
	
	for (size_t t = 0; t < T - 1; ++t) {
		//start with equilibrium at t=0;
		cout << t;
		//calculate all arc times given current flows!
		//cout << "Time: " << t << "\n";

		//check which routes are open!

		for (size_t od = 0; od < Network.numberODpairs; ++od) {
			cout << " Pathtimes: ";

			//base pathtimes on both types of drivers.
			for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
				pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][p], Schedule.arcFlowAll[t], Schedule.scheduledCapacities[t]);//patharcs, flows, caps)
				cout << pathTimes[od][p] << ' ';
			}

			//change flows only for recurring drivers (so NOT tourists!)
			for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
				Schedule.pathFlow[t + 1][od][p] = Schedule.pathFlow[t][od][p];//still excludes tourists.
				for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
					if (r != p) {
						Schedule.pathFlow[t + 1][od][p] += 0.001 * ( Schedule.pathFlow[t][od][r] * max(pathTimes[od][r] - pathTimes[od][p], 0.0) - Schedule.pathFlow[t][od][p] * max(pathTimes[od][p] - pathTimes[od][r], 0.0));
					}
				}
			}

		}
		
		//add tourists to flows at t+1, current value of arcFlowAll[t+1] is the tourist part of the flow
		findTouristFlows(Schedule.scheduledCapacities[t + 1], Network, Schedule.arcFlowAll[t + 1]);

		Schedule.addArcFlowAll(t + 1, Network.numberODpairs, Network.numberODpaths, Network.ODpaths); //assumes all flows are 0
		
		cout << "recurrentFLOW ";
		for (size_t r = 0; r < Network.numberODpaths[0]; ++r) {
			cout << Schedule.binarySchedule[t][0] << "  ";
			cout << Schedule.pathFlow[t + 1][0][r] << ' ';
		} 
		
		//for each OD pair
			//for all paths
		//determine path time given all flows (using calculated time)

		//set flow for t+1
		
		cout << '\n';
	}


	return;
}