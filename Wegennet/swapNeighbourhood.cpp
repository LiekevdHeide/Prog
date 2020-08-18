#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "VNS.h"

#include "costCalculationFunctions.h"
#include <vector>
#include <algorithm>

using namespace std;
double swapNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, vector<vector<vector<double>>>& touristAltPerwholeState, size_t numSmallStep, double bigCost) {

	double bestSoFarCosts = costFromStarttimes(Network, Maintenance, Schedule, touristAltPerwholeState, numSmallStep, bigCost);
	double currentCosts = bestSoFarCosts;
	ScheduleAndFlows currentSchedule = Schedule;
	ScheduleAndFlows bestSchedule = Schedule;

	size_t minStart;
	size_t maxEnd;
	vector<size_t> oldStartTimes = Schedule.startTimes;
	//check all swaps of 2 maintenance activities
	for (size_t m = 0; m < Maintenance.M; ++m) 
		for(size_t n = m + 1; n < Maintenance.M; ++n){
			
			minStart = min(oldStartTimes[m], oldStartTimes[n]);
			maxEnd = max(oldStartTimes[m] + Maintenance.duration[m], oldStartTimes[n] + Maintenance.duration[n]);

			currentSchedule.startTimes = oldStartTimes;
			currentSchedule.startTimes[m] = minStart + maxEnd - oldStartTimes[m] - Maintenance.duration[m];
			currentSchedule.startTimes[n] = minStart + maxEnd - oldStartTimes[n] - Maintenance.duration[n];

			shiftToOne(Maintenance.M, currentSchedule.startTimes);
			currentCosts = costFromStarttimes(Network, Maintenance, currentSchedule, touristAltPerwholeState, numSmallStep, bigCost);//uses startTimes to calc: binarySchedule, capacities, availableRoutes, touristFlows + PSAP: returns costs
			
				if (currentCosts < bestSoFarCosts) {
					bestSchedule = currentSchedule;
					//steepest descent
					bestSoFarCosts = currentCosts;
					//not steepest descent
					//break;
				}

		}

	Schedule = bestSchedule;

	return bestSoFarCosts;
}