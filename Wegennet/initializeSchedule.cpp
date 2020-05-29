#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "InitializationFunctions.h"
#include "ScheduleCheckFunctions.h"
#include "TouristAlternative.h"
#include "PSAP.h"

#include <iostream>
#include <vector>

using namespace std;

void initializeSchedule(ScheduleAndFlows &Schedule, ScheduleAndFlows &equilibrium, MaintenanceActivities &Maintenance, RoadNetwork &Network, vector<vector<vector<double>>> & touristAltPerWholeState, size_t numSmallStep){
	//create initial schedule in the ScheduleAndFlows class (binary) + adds the corresponding traffic flows (touristArcFlow, pathFlow, arcFlowAll)
	//doesn't check if it's a feasible schedule..
	
	//For now quick and easy, are better initialization heuristics +- 
	size_t t = 1; 
	for (size_t m = 0; m < Maintenance.M; ++m) {
		if (t > (Maintenance.T - Maintenance.duration[m])) {
			t = 1;
		}
		for (size_t d = 0; d < Maintenance.duration[m]; ++d) {
			Schedule.binarySchedule[t + d][m] = 1;
		}
		t += Maintenance.duration[m] ;	
	}
	//ADD CHECK IF STILL ALL DEMANDS POSSIBLE!

	//change the available capacities in the network corr to the binary schedule.
	binaryToCapacities(Maintenance.T, Maintenance.M, Schedule.binarySchedule, Maintenance.locationSets, Network.standardCapacities , Schedule.scheduledCapacities);
	adjustAvailableRoutes(Maintenance.T, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, Schedule.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, Schedule.availableRoutes, Schedule.numAvailableRoutes);
	adjustTouristArcFLows(0, Maintenance.T, Maintenance.M, Schedule.binarySchedule, touristAltPerWholeState, Schedule.arcFlowTourist);
	//set informed pathFlows / arcFlows to 0 for t> 0

	Schedule.arcFlowAll[0] = equilibrium.arcFlowAll[0];
	for(size_t od = 0; od < Network.numberODpairs; ++od)
		for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
			Schedule.pathFlow[0][od][p] = (1.00 - Network.touristPercentage) * equilibrium.pathFlow[0][od][p];
		}
	
	for (size_t t = 1; t < Maintenance.T; ++t)
		for (size_t a = 0; a < Network.vertices; ++a) {
			for (size_t b = 0; b < Network.vertices; ++b) {
				Schedule.arcFlowAll[t][a][b] = 0.0;
			}
		}
	for (size_t t = 1; t < Maintenance.T; ++t)
		for (size_t od = 0; od < Network.numberODpairs; ++od)
			for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
				Schedule.pathFlow[t][od][r] = 0.0;
			}

	adjustingTrafficFlows(Maintenance.T, Network, Schedule, numSmallStep);


	return;
}