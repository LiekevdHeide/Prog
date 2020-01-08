//checking
#include "performanceTimer.h"
#include "generalPrintingFunctions.h"
//classes
#include "RoadNetwork.h"
#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"

#include "ConvexCombinations.h"
#include "InitializationFunctions.h"
#include "PSAP.h"


#include <vector>
//#include <functional> //if want to make a standard function that can be different functions (such as policies etc)
//#include <random>
#include <iostream>
#include <fstream>

using namespace std;

int main() 
{
	string whichComputer{ "X:/My Documents/Wegennetwerk/Prog" };
	//string whichComputer{ "C:/Users/lieke/Documents/Wegennetwerk/Prog" };

	Timer time;

	//read the data file with road network    //should be inputs: road network configurations (set of vertices, arcs (directed), OD-pairs, constant for traffic time per arc)
	string roadInput = whichComputer + "/roadInputXYY.txt";

	RoadNetwork Network(roadInput);

	for (size_t n = 0; n < Network.numberODpairs; ++n) {
		print2Dim(Network.ODpaths[n], Network.numberODpaths[n]);
		cout << '\n';
	}
	//read data file with maintenance action info    //should be inputs: data for maintenance projects (location sets, durations, reduction of cap, time frame)
	string maintenanceInput = whichComputer + "/maintenanceInput.txt";
	MaintenanceActivities Maintenance(maintenanceInput, Network.vertices);

	print2Dim(Maintenance.locationSets, Maintenance.M, 2);

	//class? with schedule / capacities per link per time unit?
	ScheduleAndFlows Schedule(Maintenance.T, Network.vertices, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.standardCapacities);

	print2Dim(Schedule.binarySchedule, Maintenance.T, Maintenance.M);

	//write results to:
	ofstream write(whichComputer + "/Results.txt");  //, std::ios::app for adding to end of file

	cout << "Reading time: " << time.elapsed() << " seconds \n";
	time.reset();

	//calculate equilibrium with no maintenance scheduled.
	//CONVEX COMBINATIONS:
	ScheduleAndFlows equilibrium(2, Network.vertices, 0, Network.numberODpairs, Network.numberODpaths, Network.standardCapacities);//all flows 0, no maintenance
	convexCombinations(equilibrium, Network, 0.001, 0.0001);//convergenceCriterion,  epsilon for stepsize?
	

	//Make initial schedule solution 
	cout << "--------------Create initial schedule ----------------\n";

	//initializeSchedule(Schedule, Maintenance);
	
	Schedule.arcFlow[0][0] = {{0, 5, 5, 0}, {0, 0, 0, 5}, {0, 0, 0, 5}, {0, 0, 0, 0}};
	print2Dim(Schedule.arcFlow[0][0], Network.vertices);
	Schedule.arcFlowAll[0] = Schedule.arcFlow[0][0];
	Schedule.pathFlow[0][0] = { 5,5 };
	
	cout << "\n\n";
	//print2Dim(Schedule.binarySchedule, Maintenance.T, Maintenance.M);
	//for (size_t t = 0; t < Maintenance.T; ++t) {
	//	print2Dim(Schedule.scheduledCapacities[t], Network.vertices);
	//	cout << '\n';
	//}

	//cost function: return total travel time at a certain day, for a given schedule.


	//dynamic adjustment function: PSAP
	proportionalSwitch(Maintenance.T, Network, Schedule);

	//implement a heuristic GA/ALNS?

	cout << "\nImplementation time: " << time.elapsed() << " seconds\n";
}