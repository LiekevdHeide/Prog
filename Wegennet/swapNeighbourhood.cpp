#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "VNS.h"

#include "costCalculationFunctions.h"
#include <vector>

using namespace std;
bool swapNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, vector<vector<vector<double>>>& touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod, double bigCost) {
	bool foundImprovement = false;

	double bestSoFarCosts = totalTravelTime(Network, Maintenance.T, Schedule.scheduledCapacities, Schedule.arcFlowAll);
	double currentCosts = bestSoFarCosts;
	ScheduleAndFlows currentSchedule = Schedule;
	ScheduleAndFlows bestSchedule = Schedule;

	//print2Dim(Schedule.binarySchedule, Maintenance.T - runoutPeriod);
	
	//check all swaps of 2 maintenance activities
	for (size_t m = 0; m < Maintenance.M; ++m) 
		for(size_t n = m + 1; n < Maintenance.M; ++n){
			
			//if feasible swap:
			if (Schedule.startTimes[n] + Maintenance.duration[m] <= Maintenance.T - runoutPeriod || Schedule.startTimes[m] + Maintenance.duration[n] <= Maintenance.T - runoutPeriod) {

				//adjust binarySchedule
				currentSchedule = Schedule;
				currentSchedule.startTimes[m] = Schedule.startTimes[n];
				currentSchedule.startTimes[n] = Schedule.startTimes[m];

				currentCosts = costFromStarttimes(Network, Maintenance, currentSchedule, touristAltPerwholeState, numSmallStep, bigCost);//uses startTimes to calc: binarySchedule, capacities, availableRoutes, touristFlows + PSAP: returns costs

				if (currentCosts < bestSoFarCosts) {
					foundImprovement = true;
					bestSchedule = currentSchedule;
					//steepest descent
					//bestSoFarCosts = currentCosts;
					//not steepest descent
					break;
				}

			}
		}

	if (foundImprovement) {
		Schedule = bestSchedule;
	}

	return foundImprovement;
}