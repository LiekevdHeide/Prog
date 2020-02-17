//checking
#include "performanceTimer.h"
#include "generalPrintingFunctions.h"
//classes
#include "RoadNetwork.h"
#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"

#include "ScheduleCheckFunctions.h"

#include "ConvexCombinations.h"
#include "InitializationFunctions.h"
#include "PSAP.h"
#include "DepthFirstSearchFunctions.h"


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
	string roadInput = whichComputer + "/roadInput.txt";

	RoadNetwork Network(roadInput);
	//find all routes
	depthFirstSearch(Network);
	
	//read data file with maintenance action info    //should be inputs: data for maintenance projects (location sets, durations, reduction of cap, time frame)
	string maintenanceInput = whichComputer + "/maintenanceInput.txt";
	MaintenanceActivities Maintenance(maintenanceInput, Network.vertices, Network.numberODpairs, Network.numberODpaths);
	print2Dim(Maintenance.locationSets, Maintenance.M, 2);

	//find which maintenance interrupts which route:
	findInterruptedRoutes(Maintenance.M, Maintenance.locationSets, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, Maintenance.interruptedRoutes);
	cout << '\n';
	for (size_t m = 0; m < Maintenance.M; ++m) {
		print2Dim(Maintenance.interruptedRoutes[m], Network.numberODpairs);//immediately fails
	}

	//class? with schedule / capacities per link per time unit?
	ScheduleAndFlows Schedule(Maintenance.T, Network.vertices, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.standardCapacities);

	//print2Dim(Schedule.binarySchedule, Maintenance.T, Maintenance.M);

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

	initializeSchedule(Schedule, Maintenance);
	adjustAvailableRoutes(Maintenance.T, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, Schedule.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, Schedule.availableRoutes, Schedule.numAvailableRoutes);

	for (size_t t = 0; t < Maintenance.T; ++t) {
		cout << t << "----\n";
		print2Dim(Schedule.availableRoutes[t], Network.numberODpairs);
	}

	//start at equilibrium.
	Schedule.arcFlow[0] = equilibrium.arcFlow[0];
	Schedule.arcFlowAll[0] = equilibrium.arcFlowAll[0];
	Schedule.pathFlow[0] = equilibrium.pathFlow[0];
	
	for (size_t t = 1; t < Maintenance.T; ++t) 
		for (size_t a = 0; a < Network.vertices; ++a) {
			for (size_t b = 0; b < Network.vertices; ++b) {
				Schedule.arcFlowAll[t][a][b] = Network.touristPercentage * equilibrium.arcFlowAll[0][a][b];
			}
		}

	cout << "-----------------\n";
	cout << "Equilibrium all flows:\n";
	print2Dim(Schedule.arcFlowAll[0], Network.vertices);
	cout << "-----------------\n";
	cout << "Equilibrium tourist flows:\n";
	print2Dim(Schedule.arcFlowAll[1], Network.vertices);
	cout << "-----------------\n";
		
	//cost function: return total travel time at a certain day, for a given schedule.
	
	//dynamic adjustment function: PSAP
	//CHECK TIMING!!!
	adjustingTrafficFlows(Maintenance.T, Network, Schedule);

	//implement a heuristic GA/ALNS?

	cout << "\nImplementation time: " << time.elapsed() << " seconds\n";
}