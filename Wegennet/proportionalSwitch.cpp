
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
			pathTimes[od] = vector<double>(Network.numberODpaths[od]);//init to eq time
			//availableRoutes[od] = vector<size_t>(Network.numberODpaths[od]);//?
	}
	//vector<size_t> numAvailableRoutes(Network.numberODpairs, 0);
	
	
	vector<vector<double>> touristFlows(Network.vertices, vector<double>(Network.vertices, 0.0));

	double flowAtClosingPath = 0.0;

	//arcFlow: both drivers, pathflow: only recurrent drivers
	//arcflow input is ONLY THE TOURIST IN EQUILIBRIUM FLOW

	//pathTime is init to freeFlow time -> if not available at t, but used at t+1: time is eq time

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
		
		//check which routes are open -> Schedule.availableRoutes.

		for (size_t od = 0; od < Network.numberODpairs; ++od) {
			cout << " Pathtimes: ";
			//---------------------------Calculate pathtimes at time t

			//set all pathTimes to free flow time when no maintenance is scheduled:
			for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
				pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][p], vector<vector<double>>(Network.vertices, vector<double>(Network.vertices, 0.0)) , Network.standardCapacities);
			}

			//change pathtimes for available routes: based on both types of drivers.
			for (size_t p = 0; p < Schedule.numAvailableRoutes[t][od]; ++p) {
				pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][Schedule.availableRoutes[t][od][p]], Schedule.arcFlowAll[t], Schedule.scheduledCapacities[t]);//patharcs (for available route), flows, caps)
				cout << pathTimes[od][p] << ' ';
			}
			//----------------------------Change recurring traffic flows at t+1

			flowAtClosingPath = 0.0;
			size_t indexShortest = 0;
		   //find flows on paths at t, which are unavailable at t+1!
			for (size_t p = 0; p < Schedule.numAvailableRoutes[t][od]; ++p) {
				if (find(Schedule.availableRoutes[t + 1][od].begin(), Schedule.availableRoutes[t + 1][od].end(), Schedule.availableRoutes[t][od][p]) == Schedule.availableRoutes[t + 1][od].end()) {
					flowAtClosingPath += Schedule.pathFlow[t][od][p];
				}
				else {
					//find shortest path at t (which is available at t+1) and add flowAtClosingPath
					if (pathTimes[od][p] < pathTimes[od][indexShortest]) {
						indexShortest = p;
					}
				}
			}
	
			//change flows only for recurring drivers (so NOT tourists!), only consider changing from / to available paths at t+1
			for (size_t p = 0; p < Schedule.numAvailableRoutes[t + 1][od]; ++p) {
				//update pathflows based for all available at t + 1, based on t flows (FOR ALL AVAILABLE AT t and t + 1), only for non-tourists
				
				//if route not available at t: flow is 0.0 so OK
				Schedule.pathFlow[t + 1][od][Schedule.availableRoutes[t + 1][od][p]] = Schedule.pathFlow[t][od][Schedule.availableRoutes[t + 1][od][p]];//still excludes tourists.

				for (size_t r = 0; r < Schedule.numAvailableRoutes[t + 1][od]; ++r) { // r is about flows at previous time
					if (r != p) {
						Schedule.pathFlow[t + 1][od][Schedule.availableRoutes[t + 1][od][p]] += 0.001 * ( Schedule.pathFlow[t][od][Schedule.availableRoutes[t][od][r]] * max(pathTimes[od][r] - pathTimes[od][p], 0.0) - Schedule.pathFlow[t][od][Schedule.availableRoutes[t][od][p]] * max(pathTimes[od][p] - pathTimes[od][r], 0.0));
					}
				}
			}

			//add flowAtClosingPath to shortest path at t (that is still available)
			Schedule.pathFlow[t + 1][od][Schedule.availableRoutes[t + 1][od][indexShortest]] += flowAtClosingPath;

		}
		//------------------------------Find tourist traffic flows

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