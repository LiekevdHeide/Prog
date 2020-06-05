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

bool timeMoveNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, vector<vector<vector<double>>>& touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod) {
	bool foundImprovement = false;

	double originalCosts = totalTravelTime(Network, Maintenance.T, Schedule.scheduledCapacities, Schedule.arcFlowAll);
	double currentCosts = originalCosts;
	ScheduleAndFlows NbSched = Schedule; //(to change: binarySchedule, scheduledCapacities, availableRoutes, numAvailableRoutes, arcFlowALl, arcFLowTourists, pathFlow(informed)
	ScheduleAndFlows bestSchedule = Schedule;
	vector<vector<size_t>> noMBinarySchedule = NbSched.binarySchedule;
	size_t numSinceImproved = 0;

	//while (numSinceImproved < Maintenance.M) {
		for (size_t mToShift = 0; mToShift < Maintenance.M; ++mToShift) {
			
			size_t currentT = 0;
			for (size_t t = 0; t < Maintenance.T; ++t) {
				if (Schedule.binarySchedule[t][mToShift] == 1) {
					currentT = t;
					break;
				}
			}
			noMBinarySchedule = NbSched.binarySchedule;
			//find general empty binarySchedule
			for (size_t t = 0; t < Maintenance.T; ++t) {
				noMBinarySchedule[t][mToShift] = 0;
			}

			for (size_t t = 1; t < Maintenance.T - runoutPeriod - Maintenance.duration[mToShift]; ++t) {
				if (t != currentT && t + Maintenance.duration[mToShift] < Maintenance.T) {			//not nec??   <=? (everything in Schedule is changed => don't need to reinitialize
					//set binary schedule to zero
					NbSched.binarySchedule = noMBinarySchedule;

					//replace the binarySchedule
					for (size_t s = t; s < t + Maintenance.duration[mToShift]; ++s) {
						NbSched.binarySchedule[s][mToShift] = 1;
					}

					currentCosts = costFromSchedule(Network, Maintenance, NbSched, touristAltPerwholeState, numSmallStep);//uses binarySchedule to calc: capacities, availableRoutes, touristFlows + PSAP: returns costs

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

			if (foundImprovement) {
				Schedule = bestSchedule;
			}
			else {
				//++numSinceImproved;
			}

		}
	//}
	return foundImprovement;
}