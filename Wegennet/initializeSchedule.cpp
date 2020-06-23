#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "InitializationFunctions.h"
#include "ScheduleCheckFunctions.h"
#include "costCalculationFunctions.h"
#include "TouristAlternative.h"
#include "PSAP.h"

#include <iostream>
#include <random>
#include <vector>

using namespace std;

void initializeSchedule(ScheduleAndFlows &Schedule, ScheduleAndFlows &equilibrium, MaintenanceActivities &Maintenance, RoadNetwork &Network, vector<vector<vector<double>>> & touristAltPerWholeState, size_t numSmallStep, size_t runoutTime, double bigCosts){
	//create initial schedule in the ScheduleAndFlows class (binary) + adds the corresponding traffic flows (touristArcFlow, pathFlow, arcFlowAll)
	//doesn't check if it's a feasible schedule..

	//set random startTimes
	random_device rd;
	mt19937 randomGenerator(rd());
	for (size_t m = 0; m < Maintenance.M; ++m) {
		uniform_int_distribution<mt19937::result_type> chooseT(1, Maintenance.T - runoutTime - Maintenance.duration[m]);
		Schedule.startTimes[m] = chooseT(randomGenerator);
	}
	shiftToOne(Maintenance.M, Schedule.startTimes);

	Schedule.arcFlowAll[0] = equilibrium.arcFlowAll[0];
	for (size_t od = 0; od < Network.numberODpairs; ++od)
		for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
			Schedule.pathFlow[0][od][p] = (1.00 - Network.touristPercentage) * equilibrium.pathFlow[0][od][p];
		}

	//update binary, adjustavailable, adjustTourists, adjustingTrafficFlows + returns Costs!
	costFromStarttimes(Network, Maintenance, Schedule, touristAltPerWholeState, numSmallStep, bigCosts);//bigCost??

	
	return;
}