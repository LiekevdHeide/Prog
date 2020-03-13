
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

		//------------------------------Find tourist traffic flows
		//add alternative routes for tourists to flows at t+1, current value of arcFlowAll[t+1] is the tourist part of the flow
		//adjust arcFlowAll t + 1 to incorporate rerouting tourists
		findAlternativeArcFlowsTourists(Network, Schedule.scheduledCapacities[t + 1], Schedule.arcFlowAll[t + 1]);

		
		//------------------------------Recurrent drivers updating:

		//at closing paths:
		for (size_t od = 0; od < Network.numberODpairs; ++od) {
			//Calculate pathtimes at time t (to base decisions t + 1 on)    based on arcFlowAll @t + currentCap	(set to freeFlow t for paths unavailable at t)	
			updateExpectedPathTimes(Network, od, Schedule.numAvailableRoutes[t][od], Schedule.availableRoutes[t][od], Schedule.scheduledCapacities[t], Schedule.arcFlowAll[t], pathTimes[od]);

			//determine recurrent flow @ closing paths
			flowAtClosingPath = determineFlowClosingPaths(Schedule, t, od);

			//Find (expected) shortest path for t + 1 using pathTimes(so shortest at t & freeflow times on newly opened ones). Is index for availablePahts at t + 1
			size_t indexShortest = findAlternativePathRecurringDrivers(Schedule, t, od, pathTimes);

			//add flowAtClosingPath to shortest expected path at t+ 1
			Schedule.pathFlow[t + 1][od][Schedule.availableRoutes[t + 1][od][indexShortest]] += flowAtClosingPath;
		}


		//at paths available at t + 1, do small steps:
		size_t numSmallStep = 10;
		vector<vector<double>> oldFlow = Schedule.pathFlow[t];//od, path
		vector<vector<double>> newFlow = Schedule.pathFlow[t];//od, path
		vector<vector<double>> arcFlows = Schedule.arcFlowAll[t];

		for (size_t s = 0; s < numSmallStep; ++s) {
			//find flows for small steps
			for (size_t od = 0; od < Network.numberODpairs; ++od) {
				//Calculate pathtimes at time t (to base decisions t + 1 on)    based on arcFlowAll @t + currentCap	(set to freeFlow t for paths unavailable at t)	
				updateExpectedPathTimes(Network, od, Schedule.numAvailableRoutes[t][od], Schedule.availableRoutes[t][od], Schedule.scheduledCapacities[t], arcFlows, pathTimes[od]);

				//Change recurring traffic flows at t+1 (using only paths available at t + 1). Resets pathFlow at t + 1 to flow at t, then updates it using PSAP.
				//adjust newFlow using pathTimes. Only change flows on/to open paths at t + 1. resets newFlow + updates it.
				proportionalSwitch(Schedule.numAvailableRoutes[t + 1][od], Schedule.availableRoutes[t + 1][od], oldFlow[od], pathTimes[od], newFlow[od]);

			}
			oldFlow = newFlow;
			//update arcFLows
			//first reset to tourist flows:
			arcFlows = Schedule.arcFlowAll[t + 1]; 
			//!!!! ERRORRRR!!!! now recurrents "know" new tourist flows..
			for(size_t od = 0; od < Network.numberODpairs; ++od)
				for(size_t p = 0; p < Network.numberODpaths[od]; ++p)
					for (size_t a = 0; a < Network.ODpaths[od][p].size() - 1; ++a) {
						arcFlows[Network.ODpaths[od][p][a]][Network.ODpaths[od][p][a + 1]] += newFlow[od][p];
					}
		}
		
		//add flow to available paths at t + 1
		for (size_t od = 0; od < Network.numberODpairs; ++od)
			for (size_t p = 0; p < Schedule.numAvailableRoutes[t + 1][od]; ++p)
				Schedule.pathFlow[t + 1][od][p] += newFlow[od][p];


		//from path flows to arc flows:			 updates Schedule.arcFlowAll[t+1] using pathflow[t+1]
		//adjust arcFlowAll (which already contains touristFlows), using pathFlow[t+1] (which is the recurrent flows)
		Schedule.addArcFlowAll(t + 1, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);

		//cout << '\n';
	}


	return;
}