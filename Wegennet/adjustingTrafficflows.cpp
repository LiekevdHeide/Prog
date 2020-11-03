
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"
#include "PSAP.h"

#include "generalPrintingFunctions.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstddef> //necessery for size_t!!
#include <iomanip> // set precision
using namespace std;

void adjustingTrafficFlows(size_t T, RoadNetwork& Network, ScheduleAndFlows& Schedule, size_t numSmallStep) { //assumes at 0 no maintenance!

	//cout << setprecision(3);

	vector<vector<double>> pathTimes(Network.numberODpairs, vector<double>());
	
	for (size_t od = 0; od < Network.numberODpairs; ++od) {
		pathTimes[od] = vector<double>(Network.numberODpaths[od]);//init to eq time ????
		//init to freeFlow time
		for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
			pathTimes[od][p] = Network.pathTravelTime(Network.ODpaths[od][p], Schedule.arcFlowAll[0], Network.standardCapacities); // vector<vector<double>>(Network.vertices, vector<double>(Network.vertices, 0.0))
		}
	}
	vector<vector<double>> freeFlowTimes = pathTimes;

	double flowAtClosingPath = 0.0;
	//check which routes are open -> Schedule.availableRoutes.

	//arcFlow: both drivers, pathflow: only recurrent drivers, tourist pathflow: only tourists
	//arcflow input is 0 flow!

	//pathTime is init to freeFlow time -> if not available at t, but used at t+1: time is eq time
	//pathTime is used to determine flows at t + 1 (so equals path durations at t)

	//allArcFlows + arcFlows still include tourists! (pathFlow does not)

	for (size_t t = 0; t < T - 1; ++t) {
		//start with equilibrium at t=0;

		//already found the cplex tourist flows (are in Schedule.arcFlowTourists)

		//------------------------------Recurrent drivers updating:
		//at closing paths:
		for (size_t od = 0; od < Network.numberODpairs; ++od) {
			//reset pathTimes: set all pathTimes to free flow time: (set to freeFlow t for ALL paths, then use updateExpected to change for all available
			pathTimes = freeFlowTimes;
			//Calculate pathtimes at time t (to base decisions t + 1 on)    based on arcFlowAll @t + currentCap	 ONLY FOR AVAILABLE PATHS!
			updateExpectedPathTimes(Network, od, Schedule.numAvailableRoutes[t][od], Schedule.availableRoutes[t][od], Schedule.scheduledCapacities[t], Schedule.arcFlowAll[t], pathTimes[od]);
			
			//if path just opened:
			size_t q = 0;
			double expectedFlow = 0.0;
			for (size_t p = 0; p < Schedule.numAvailableRoutes[t + 1][od]; ++p) {
				q = Schedule.availableRoutes[t + 1][od][p];
				if (find(Schedule.availableRoutes[t][od].begin(), Schedule.availableRoutes[t][od].end(), Schedule.availableRoutes[t + 1][od][p]) == Schedule.availableRoutes[t][od].end()) {
					//update costs: arcFlowAll[t] if >0, else: eq flows
					cout << "opening path:" << q << ' ';
					pathTimes[od][q] = 0.0;
					for (size_t route = 0; route < Network.ODpaths[od][q].size() - 1; ++route) {
						if (Schedule.arcFlowAll[t][Network.ODpaths[od][q][route]][Network.ODpaths[od][q][route + 1]] == 0) {
							expectedFlow = Schedule.arcFlowAll[0][Network.ODpaths[od][q][route]][Network.ODpaths[od][q][route + 1]];
							//cout << 0 << ' ';
						}
						else {
							expectedFlow = Schedule.arcFlowAll[t][Network.ODpaths[od][q][route]][Network.ODpaths[od][q][route + 1]];
						}
						//cout << expectedFlow << ' ';
						pathTimes[od][q] += Network.travelTimeRoad(Schedule.scheduledCapacities[t + 1][Network.ODpaths[od][q][route]][Network.ODpaths[od][q][route + 1]], Network.freeFlowTimes[Network.ODpaths[od][q][route]][Network.ODpaths[od][q][route + 1]], expectedFlow);
					}
					//cout << '\n';
				}
				cout << od << ':' << q << ' ' << pathTimes[od][q] << ' ';
			}
			cout << '\n'; 

			//determine recurrent flow @ closing paths
			flowAtClosingPath = determineFlowClosingPaths(Schedule, t, od);

			//Find (expected) shortest path for t + 1 using pathTimes(so shortest at t & freeflow times on newly opened ones). Is index for availablePahts at t + 1
			size_t indexShortest = findAlternativePathRecurringDrivers(Schedule, t, od, pathTimes);

			//add flowAtClosingPath to shortest expected path at t+ 1
			Schedule.pathFlow[t + 1][od][Schedule.availableRoutes[t + 1][od][indexShortest]] += flowAtClosingPath;
		}


		//at paths available at t + 1, do small steps:
		
		vector<vector<double>> oldFlow = Schedule.pathFlow[t];//od, path
		vector<vector<double>> newFlow = Schedule.pathFlow[t];//od, path
		vector<vector<double>> arcFlows = Schedule.arcFlowAll[t];

		for (size_t s = 0; s < numSmallStep; ++s) {
			//find flows for small steps
			for (size_t od = 0; od < Network.numberODpairs; ++od) {
				//Calculate pathtimes at time t (to base decisions t + 1 on)    based on arcFlowAll @t + currentCap	(set to freeFlow t for paths unavailable at t)	
				pathTimes = freeFlowTimes;
				updateExpectedPathTimes(Network, od, Schedule.numAvailableRoutes[t][od], Schedule.availableRoutes[t][od], Schedule.scheduledCapacities[t], arcFlows, pathTimes[od]);
				//updateExpectedPathTimes(Network, od, Schedule.numAvailableRoutes[t][od], Schedule.availableRoutes[t][od], Schedule.scheduledCapacities[t], arcFlows, pathTimes[od]);
				
				//Change recurring traffic flows at t+1 (using only paths available at t + 1). Resets pathFlow at t + 1 to flow at t, then updates it using PSAP.
				//adjust newFlow using pathTimes. Only change flows on/to open paths at t + 1. resets newFlow + updates it.
				proportionalSwitch(Schedule.numAvailableRoutes[t + 1][od], Schedule.availableRoutes[t + 1][od], oldFlow[od], pathTimes[od], newFlow[od]);

			}
			oldFlow = newFlow;
			//update arcFLows
			//first reset to tourist flows:
			arcFlows = Schedule.arcFlowTourist[t]; 
			for(size_t od = 0; od < Network.numberODpairs; ++od)
				for(size_t p = 0; p < Network.numberODpaths[od]; ++p)
					for (size_t a = 0; a < Network.ODpaths[od][p].size() - 1; ++a) {
						arcFlows[Network.ODpaths[od][p][a]][Network.ODpaths[od][p][a + 1]] += newFlow[od][p];
					}
		}
		
		//add flow to available paths at t + 1
		for (size_t od = 0; od < Network.numberODpairs; ++od)
			for (size_t p = 0; p < Schedule.numAvailableRoutes[t + 1][od]; ++p)
				Schedule.pathFlow[t + 1][od][Schedule.availableRoutes[t + 1][od][p]] += newFlow[od][Schedule.availableRoutes[t + 1][od][p]];


		//from path flows to arc flows:			 updates Schedule.arcFlowAll[t+1] using pathflow[t+1]
		Schedule.arcFlowAll[t + 1] = Schedule.arcFlowTourist[t + 1];
		
		//adjust arcFlowAll (which already contains touristFlows), using pathFlow[t+1] (which is the recurrent flows)
		Schedule.addArcFlowAll(t + 1, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);
		//cout << '\n';
	}


	return;
}