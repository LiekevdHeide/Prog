#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "VNS.h"

#include "costCalculationFunctions.h"


#include <vector>
#include <algorithm>

void timeMoveNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, vector<vector<vector<double>>>& touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod, double bigCost) {

	double bestCosts = totalTravelTime(Network, Maintenance.T, Schedule.scheduledCapacities, Schedule.arcFlowAll);
	double currentCosts = bestCosts;
	ScheduleAndFlows NbSched = Schedule; 
	ScheduleAndFlows bestSchedule = Schedule;
	vector<size_t> originalstartTimes = Schedule.startTimes;

		for (size_t mToShift = 0; mToShift < Maintenance.M; ++mToShift) {
			for (size_t t = 1; t < Maintenance.T - runoutPeriod - Maintenance.duration[mToShift]; ++t) {
				if (t != Schedule.startTimes[mToShift]) {			//  <=? 

					//set startTime to t
					NbSched.startTimes[mToShift] = t;

					shiftToOne(Maintenance.M, NbSched.startTimes);
					currentCosts = costFromStarttimes(Network, Maintenance, NbSched, touristAltPerwholeState, numSmallStep, bigCost);//uses binarySchedule to calc: capacities, availableRoutes, touristFlows + PSAP: returns costs

					 //if found improvement:
					if (currentCosts < bestCosts) {
						bestSchedule = NbSched;
						//steepest descent
						bestCosts = currentCosts;

						//not steepest descent
						//break;
					}

				}
				NbSched.startTimes = originalstartTimes;
			}

		}

	Schedule = bestSchedule;

	return;
}