#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include "InitializationFunctions.h"
#include "ScheduleCheckFunctions.h"

#include <iostream>

void initializeSchedule(ScheduleAndFlows &Schedule, MaintenanceActivities &Maintenance, RoadNetwork &Network){
	//create initial schedule in the ScheduleAndFlows class (binary)
	
	//For now quick and easy, are better initialization heuristics +- 
	size_t t = 10; 
	for (size_t m = 0; m < Maintenance.M; ++m) {
		if (t > (Maintenance.T - Maintenance.duration[m])) {
			t = 10;
		}
		cout << Maintenance.duration[m] << ' ';
		for (size_t d = 0; d < Maintenance.duration[m]; ++d) {
			Schedule.binarySchedule[t + d][m] = 1;
		}
		t += Maintenance.duration[m] ;	
	}
	//ADD CHECK IF STILL ALL DEMANDS POSSIBLE!

	//change the available capacities in the network corr to the binary schedule.
	binaryToCapacities(Maintenance.T, Maintenance.M, Schedule.binarySchedule, Maintenance.locationSets, Network.standardCapacities , Schedule.scheduledCapacities);

	return;
}