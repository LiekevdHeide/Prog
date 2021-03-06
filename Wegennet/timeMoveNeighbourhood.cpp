#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "VNS.h"

#include "costCalculationFunctions.h"

#include <cstddef> //necessery for size_t!!
#include <vector>
#include <algorithm>

double timeMoveNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, vector<vector<vector<double>>>& touristAltPerwholeState, double PSAPalpha, size_t numSmallStep, double bigCost) {

	double bestCosts = costFromStarttimes(Network, Maintenance, Schedule, touristAltPerwholeState, PSAPalpha, numSmallStep, bigCost);
	double currentCosts = bestCosts;
	ScheduleAndFlows NbSched = Schedule; 
	ScheduleAndFlows bestSchedule = Schedule;
	vector<size_t> originalstartTimes = Schedule.startTimes;

		for (size_t mToShift = 0; mToShift < Maintenance.M; ++mToShift) {
			for (size_t t = 1; t < Maintenance.T - Maintenance.runOutPeriod - Maintenance.duration[mToShift]; ++t) {
				if (t != Schedule.startTimes[mToShift]) {			//  <=? 

					//set startTime to t
					NbSched.startTimes[mToShift] = t;

					shiftToOne(Maintenance.M, NbSched.startTimes);
					currentCosts = costFromStarttimes(Network, Maintenance, NbSched, touristAltPerwholeState, PSAPalpha, numSmallStep, bigCost);//uses binarySchedule to calc: capacities, availableRoutes, touristFlows + PSAP: returns costs

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

	return bestCosts;
}