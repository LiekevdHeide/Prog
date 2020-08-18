
#include "RoadNetwork.h"
#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"

#include "PSAP.h"
#include "TouristAlternative.h"
#include "costCalculationFunctions.h"
#include "ScheduleCheckFunctions.h"

#include <vector>
#include <cstddef> //necessery for size_t!!

using namespace std;
double costFromSchedule(RoadNetwork &Network, MaintenanceActivities& Maintenance, ScheduleAndFlows& Schedule, vector<vector<vector<double>>>& touristAltPerwholeState, size_t numSmallStep, double bigCost) {
	//already changed: binarySchedule

	//(to change: scheduledCapacities, availableRoutes, numAvailableRoutes, arcFlowALl, arcFLowTourists, pathFlow(informed)

	binaryToCapacities(Maintenance.T, Maintenance.M, Schedule.binarySchedule, Maintenance.locationSets, Network.standardCapacities, Schedule.scheduledCapacities);//changes to scheduledCapacities to correspond with binarySchedule

	double costs;
	//replace the available routes + numAvailableRoutes
	if (adjustAvailableRoutes(Maintenance.T, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, Schedule.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, Schedule.availableRoutes, Schedule.numAvailableRoutes)) {

		//adjust uninformed routing in arcFlowTourists
		adjustTouristArcFLows(Maintenance.T, Maintenance.M, Schedule.binarySchedule, touristAltPerwholeState, Schedule.arcFlowTourist);//uses assignment, not +=

		//keep traffic flows at t=0 the same! since always in equilibrium

		//set informed pathFlows / arcFlows to 0 for t> 0
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

		//add arcFLowTourist to arcFlowAll? / check costFunction in adjustingTrafficFlows to see if it includes tourists


		//Adjust informed routing (pathFLow)
		adjustingTrafficFlows(Maintenance.T, Network, Schedule, numSmallStep);



		//calculate new costs:
		costs = totalTravelTime(Network, Maintenance.T, Schedule.scheduledCapacities, Schedule.arcFlowAll);
	}
	else {
		costs = bigCost;//BIG M since infeasible
	}

	return costs;
}