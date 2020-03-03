
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"
#include "PSAP.h"

#include "generalPrintingFunctions.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip> // set precision
using namespace std;

void adjustingTrafficFlows(size_t T, RoadNetwork& Network, ScheduleAndFlows& Schedule) { //assumes at 0 no maintenance!

	//cout << setprecision(3);

	vector<vector<double>> pathTimes(Network.numberODpairs, vector<double>());
	
	for (size_t od = 0; od < Network.numberODpairs; ++od) {
		pathTimes[od] = vector<double>(Network.numberODpaths[od]);//init to eq time
	}

	//vector<vector<double>> touristFlows(Network.vertices, vector<double>(Network.vertices, 0.0));//tourist flows per arc

	double flowAtClosingPath = 0.0;
	//check which routes are open -> Schedule.availableRoutes.

	//arcFlow: both drivers, pathflow: only recurrent drivers
	//arcflow input is ONLY THE TOURIST IN EQUILIBRIUM FLOW

	//pathTime is init to freeFlow time -> if not available at t, but used at t+1: time is eq time
	//pathTime is used to determine flows at t + 1 (so equals path durations at t)

	//subtract the tourists from the path flows:
	//cout << "recFLOW ";
	for (size_t od = 0; od < Network.numberODpairs; ++od) {
		//cout << "od:" << od << "  ";
		for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
			Schedule.pathFlow[0][od][r] *= (1 - Network.touristPercentage);
			//cout << Schedule.pathFlow[0][od][r] << ' ';
		}
		//cout << ' ';
	}
	//allArcFlows + arcFlows still include tourists! (pathFlow does not)

	for (size_t t = 0; t < T - 1; ++t) {
		//start with equilibrium at t=0;
		//cout << t + 1 << ' ';

		//for (size_t m = 0; m < Schedule.binarySchedule[t].size(); ++m) {
		//	cout << Schedule.binarySchedule[t + 1][m];
		//}
		//cout << ' ';

		//------------------------------Find tourist traffic flows


		//add alternative routes for tourists to flows at t+1, current value of arcFlowAll[t+1] is the tourist part of the flow

		//adjust arcFlowAll
		findAlternativeArcFlowsTourists(Network, Schedule.scheduledCapacities[t + 1], Schedule.arcFlowAll[t + 1]);

		
		//------------------------------Recurrent drivers updating:
		for (size_t od = 0; od < Network.numberODpairs; ++od) {
			//cout << " Pt-1: ";
			//Calculate pathtimes at time t (to base decisions t + 1 on)			
			updateExpectedPathTimes(Network, Schedule, t, od, pathTimes);
			
			flowAtClosingPath = determineFlowClosingPaths(Schedule, t, od);
			//cout << "CL:" << flowAtClosingPath << ' ';
			//Find (expected) shortest path for t + 1 (so shortest at t + freeflow times on newly opened ones)
			size_t indexShortest = findAlternativePathRecurringDrivers(Schedule, t, od, pathTimes);

			//Change recurring traffic flows at t+1
			proportionalSwitch(Schedule, t, od, pathTimes);
			
			//add flowAtClosingPath to shortest expected path at t+ 1
			Schedule.pathFlow[t + 1][od][Schedule.availableRoutes[t + 1][od][indexShortest]] += flowAtClosingPath;

		}
		
		
		//from path flows to arc flows:			 updates Schedule.arcFlowAll[t+1] using pathflow[t+1]
		//adjust arcFlowAll (which already contains touristFlows), using pathFlow[t+1] (which is the recurrent flows)
		Schedule.addArcFlowAll(t + 1, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);

		//cout << '\n';
	}


	return;
}