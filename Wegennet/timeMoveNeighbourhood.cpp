#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "VNS.h"
#include "PSAP.h"
#include "TouristAlternative.h"
#include "costCalculationFunctions.h"
#include "ScheduleCheckFunctions.h"

#include <vector>
#include <algorithm>

bool timeMoveNeighbourhood(RoadNetwork &Network, ScheduleAndFlows &Schedule, MaintenanceActivities &Maintenance, vector<vector<vector<double>>> &touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod, size_t mToShift) {
	bool foundImprovement = false;

	double originalCosts = totalTravelTime(Network, Maintenance.T, Schedule.scheduledCapacities, Schedule.arcFlowAll);
	double currentCosts = originalCosts;
	ScheduleAndFlows NbSched = Schedule; //(to change: binarySchedule, scheduledCapacities, availableRoutes, numAvailableRoutes, arcFlowALl, arcFLowTourists, pathFlow(informed)
	ScheduleAndFlows bestSchedule = Schedule;
	vector<vector<size_t>> noMBinarySchedule = NbSched.binarySchedule;

	size_t currentT = 0;
	for (size_t t = 0; t < Maintenance.T; ++t) {
		if (Schedule.binarySchedule[t][mToShift] == 1) {
			currentT = t;
			break;
		}
	}

	//find general empty binarySchedule
	for (size_t t = 0; t < Maintenance.T; ++t) {
		noMBinarySchedule[t][mToShift] = 0;
	}

	//for (size_t t = max(int(currentT - 5), 1); t <= min(currentT + 5, Maintenance.T); ++t) {
	for(size_t t = 1; t < Maintenance.T - runoutPeriod - Maintenance.duration[mToShift]; ++t){
		if (t != currentT && t + Maintenance.duration[mToShift] < Maintenance.T) {			// <=? (everything in Schedule is changed => don't need to reinitialize
			//set binary schedule to zero
			NbSched.binarySchedule = noMBinarySchedule;

			//replace the binarySchedule
			for (size_t s = t; s < t + Maintenance.duration[mToShift]; ++s) {
				NbSched.binarySchedule[s][mToShift] = 1;
			}
			//replace scheduledCapacities (!!reprogram so from certain time onwards)
			binaryToCapacities(Maintenance.T, Maintenance.M, NbSched.binarySchedule, Maintenance.locationSets, Network.standardCapacities, NbSched.scheduledCapacities);
			
			//replace the available routes + numAvailableRoutes
			if (adjustAvailableRoutes(Maintenance.T, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, NbSched.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, NbSched.availableRoutes, NbSched.numAvailableRoutes)) {

				//adjust uninformed routing in arcFlowTourists
				adjustTouristArcFLows(Maintenance.T, Maintenance.M, NbSched.binarySchedule, touristAltPerwholeState, NbSched.arcFlowTourist);//uses assignment, not +=

				//set informed pathFlows / arcFlows to 0 for t> 0
				for (size_t t = 1; t < Maintenance.T; ++t)
					for (size_t a = 0; a < Network.vertices; ++a) {
						for (size_t b = 0; b < Network.vertices; ++b) {
							NbSched.arcFlowAll[t][a][b] = 0.0;
						}
					}
				for (size_t t = 1; t < Maintenance.T; ++t)
					for (size_t od = 0; od < Network.numberODpairs; ++od)
						for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
							NbSched.pathFlow[t][od][r] = 0.0;
						}

				//add arcFLowTourist to arcFlowAll? / check costFunction in adjustingTrafficFlows to see if it includes tourists


				//Adjust informed routing (pathFLow)
				adjustingTrafficFlows(Maintenance.T, Network, NbSched, numSmallStep);

				//+add to allFlow?


				//calculate new costs:
				currentCosts = totalTravelTime(Network, Maintenance.T, NbSched.scheduledCapacities, NbSched.arcFlowAll);
				cout << currentCosts << ' ';
				//if found improvement:
				if (currentCosts < originalCosts) {
					foundImprovement = true;
					bestSchedule = NbSched;

					//steepest descent
					originalCosts = currentCosts;

					//not steepest descent
					//break;
				}
			}
		}
	}
	if (foundImprovement) {
		Schedule = bestSchedule;
	}

	return foundImprovement;
}