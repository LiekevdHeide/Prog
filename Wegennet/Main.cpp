//checking
#include "performanceTimer.h"
#include "generalPrintingFunctions.h"
//classes
#include "RoadNetwork.h"
#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"

#include "ScheduleCheckFunctions.h"
#include "costCalculationFunctions.h"
#include "printingResults.h"

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
	//string whichComputer{ "X:/My Documents/Wegennetwerk/Experiment 2/" };
	string whichComputer{ "C:/Users/Gebruiker/Documents/Wegennetwerk/Experiment small/" };

	Timer time;

	//read the data file with road network    //should be inputs: road network configurations (set of vertices, arcs (directed), OD-pairs, constant for traffic time per arc)
	string roadInput = whichComputer + "NetworkInputExperimentSmall.txt";

	RoadNetwork Network(roadInput);
	//find all routes
	depthFirstSearch(Network);

	//read data file with maintenance action info    //should be inputs: data for maintenance projects (location sets, durations, reduction of cap, time frame)
	string maintenanceInput = whichComputer + "maintenanceInputExperimentSmall.txt";
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
	ofstream write(whichComputer + "ResultsB.txt");  //, std::ios::app for adding to end of file
	write << roadInput << ' ' << maintenanceInput << '\n';
	printRoutes(write, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);

	cout << "Reading time: " << time.elapsed() << " seconds \n";
	time.reset();

	//calculate equilibrium with no maintenance scheduled.
	//CONVEX COMBINATIONS:
	ScheduleAndFlows equilibrium(2, Network.vertices, 0, Network.numberODpairs, Network.numberODpaths, Network.standardCapacities);//all flows 0, no maintenance
	convexCombinations(equilibrium, Network, 0.001, 0.0001, 0);//convergenceCriterion,  epsilon for stepsize?, time!
	
	print2Dim(equilibrium.arcFlowAll[0]);


	//Make initial schedule solution 
	cout << "--------------Create initial schedule ----------------\n";

	initializeSchedule(Schedule, Maintenance, Network);
	adjustAvailableRoutes(Maintenance.T, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, Schedule.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, Schedule.availableRoutes, Schedule.numAvailableRoutes);

	double currentCosts = 0.0;
	double bestCosts = 0.0;
	double worstCosts = 0.0;
	ScheduleAndFlows bestSchedule(Schedule);
	ScheduleAndFlows worstSchedule(Schedule);

	size_t runOutPeriod = 10;

	//for all new schedules?
	for (size_t s = 0; s < pow(Maintenance.T - runOutPeriod, Maintenance.M); s++) { // s < time periods ^ maintenance activities = pow(Maint.T, Maint.M) //pow(Maintenance.T, Maintenance.M)
		//adjust schedule
		if (bruteForceSchedule(Schedule, Maintenance, Network, s, runOutPeriod)) {//start from t = 1 (t = 0 is equilibrium!) (also adjustsavailableRoute)

			//cout << "-----------------\n";
			cout << s << ':';
			//start at equilibrium.
			Schedule.arcFlowAll[0] = equilibrium.arcFlowAll[0];
			for(size_t t = 0; t < Maintenance.T; ++t)
			for(size_t a = 0; a < Network.vertices; ++a)
				for (size_t b = 0; b < Network.vertices; ++b) {
					Schedule.arcFlowTourist[t][a][b] = equilibrium.arcFlowAll[0][a][b] * Network.touristPercentage;
				}
			for(size_t od = 0; od < Network.numberODpairs; ++od)
				for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
					Schedule.pathFlow[0][od][p] = equilibrium.pathFlow[0][od][p] * (1.00 - Network.touristPercentage);
					Schedule.touristPathFlow[0][od][p] = equilibrium.touristPathFlow[0][od][p] * Network.touristPercentage;
				}

			//set to 0 for next ones
			for (size_t t = 1; t < Maintenance.T; ++t)
				for (size_t a = 0; a < Network.vertices; ++a) {
					for (size_t b = 0; b < Network.vertices; ++b) {
						Schedule.arcFlowAll[t][a][b] = 0.0;// Network.touristPercentage* equilibrium.arcFlowAll[0][a][b];
					}
				}
			for (size_t t = 1; t < Maintenance.T; ++t) 
				for(size_t od = 0; od < Network.numberODpairs; ++od)
					for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
						Schedule.pathFlow[t][od][r] = 0.0;
						Schedule.touristPathFlow[t][od][r] = 0.0;
					}

			//dynamic adjustment function: PSAP
			//CHECK TIMING!!!
			adjustingTrafficFlows(Maintenance.T, Network, Schedule);

			for (size_t t = 0; t < Maintenance.T; ++t) {
				print2Dim(Schedule.pathFlow[t]);
				//cout << "t";
				//print2Dim(Schedule.touristPathFlow[t]);
			}
			/*for(size_t t = 0; t < Maintenance.T; ++t)
				for(size_t a = 0; a < Network.vertices; ++a)
					for (size_t b = 0; b < Network.vertices; ++b) {
						if (Schedule.arcFlowTourist[t][a][b] != equilibrium.arcFlowAll[0][a][b] * Network.touristPercentage) {
							cout << t << ' ' << a << b << "  ";
						}
					}*/

			currentCosts = costsSchedule(Network, Maintenance.T, Schedule.scheduledCapacities, Schedule.arcFlowAll);
			cout << currentCosts << "\n";
			write << s << ' ' <<  currentCosts << '\n';
			printSchedule(write, Maintenance.T, Maintenance.M, Schedule.binarySchedule);
			write << '\n';
			//printTraffic(write, Maintenance.T, Network.vertices, Schedule.arcFlowAll);
			printRecurringTraffic(write, Maintenance.T, Network.numberODpairs, Network.numberODpaths, Schedule.pathFlow);
			write << '\n';
			
			write << "++++++++++++++++++++++++++++\n";

			if (currentCosts < bestCosts || bestCosts == 0) {
				bestCosts = currentCosts;
				bestSchedule = Schedule;
			}
			if (currentCosts > worstCosts) {
				worstCosts = currentCosts;
				worstSchedule = Schedule;
			}

			//cost function: return total travel time + joint costs of maintenance
			
			//save cost
			//check if "best"?
		}
		else {
			cout << ' ';// "IMPOSSIBLE:" << s << ' ';

		}
		
	}
	//implement a heuristic GA/ALNS?

	ofstream bestSolution(whichComputer + "BestSolution1B.txt");
	printRoutes(bestSolution, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);
	printSchedule(bestSolution, Maintenance.T, Maintenance.M, bestSchedule.binarySchedule);
	printTraffic(bestSolution, Maintenance.T, Network.vertices, bestSchedule.arcFlowAll);
	printRecurringTraffic(bestSolution, Maintenance.T, Network.numberODpairs, Network.numberODpaths, bestSchedule.pathFlow);
	bestSolution << bestCosts;

	ofstream worstSolution(whichComputer + "WorstSolution1B.txt");
	printRoutes(worstSolution, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);
	printSchedule(worstSolution, Maintenance.T, Maintenance.M, worstSchedule.binarySchedule);
	printTraffic(worstSolution, Maintenance.T, Network.vertices, worstSchedule.arcFlowAll);
	printRecurringTraffic(worstSolution, Maintenance.T, Network.numberODpairs, Network.numberODpaths, worstSchedule.pathFlow);
	worstSolution << worstCosts;

	//----------------------------------------------------------------
	//get benchmark: flows always in equilibrium (for best solution)
	ScheduleAndFlows equilibriumBenchmark(bestSchedule);
	cout << "Benchmark all equilirbium: ";
	//get all start times: (BRUTE FORCE??)
	for (size_t t = 1; t < Maintenance.T; ++t) { //0 is always equilibrium.
		if (equilibriumBenchmark.binarySchedule[t] != equilibriumBenchmark.binarySchedule[t - 1]) {
			//if schedule has changed, find new equilibrium
				for (size_t a = 0; a < Network.vertices; ++a) {
					for (size_t b = 0; b < Network.vertices; ++b) {
						equilibriumBenchmark.arcFlowAll[t][a][b] = 0.0;
						//arcFLowTourist
					}
				}

				for (size_t od = 0; od < Network.numberODpairs; ++od)
					for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
						equilibriumBenchmark.pathFlow[t][od][r] = 0.0;
						equilibriumBenchmark.touristPathFlow[t][od][r] = 0.0;
					}

			convexCombinations(equilibriumBenchmark, Network, 0.001, 0.0001, t);//convergenceCriterion,  epsilon for stepsize?

		}
		else {
			equilibriumBenchmark.arcFlowAll[t] = equilibriumBenchmark.arcFlowAll[t - 1];
		}
	}
	cout << costsSchedule(Network, Maintenance.T, equilibriumBenchmark.scheduledCapacities, equilibriumBenchmark.arcFlowAll) << "  ";
	ofstream allEq(whichComputer + "allEqBenchmarkB.txt");
	printRoutes(allEq, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);
	printSchedule(allEq, Maintenance.T, Maintenance.M, equilibriumBenchmark.binarySchedule);
	printTraffic(allEq, Maintenance.T, Network.vertices, equilibriumBenchmark.arcFlowAll);
	allEq << costsSchedule(Network, Maintenance.T, equilibriumBenchmark.scheduledCapacities, equilibriumBenchmark.arcFlowAll);

	//---------------------------------------------------------------
	//brute force opt for allEqual
	double currentEqCosts = 0.0;
	double bestEqCosts = 0.0;
	ScheduleAndFlows eqSchedule(Schedule);
	ScheduleAndFlows bestAllEqSchedule(Schedule);
	cout << "\n Best all equilibrium schedule:\n";
	//for all new schedules?
	for (size_t s = 0; s < pow(Maintenance.T - runOutPeriod, Maintenance.M); s++) { // s < time periods ^ maintenance activities = pow(Maint.T, Maint.M) //
		//adjust schedule
		cout << s << ':';
		if (bruteForceSchedule(eqSchedule, Maintenance, Network, s, runOutPeriod)) {//start from t = 1 (t = 0 is equilibrium!) (also adjustsavailableRoute)
			eqSchedule.arcFlowAll[0] = equilibrium.arcFlowAll[0];
			eqSchedule.pathFlow[0] = equilibrium.pathFlow[0];

			//just as above with bestSchedule...
			for (size_t t = 1; t < Maintenance.T; ++t) { //0 is always equilibrium.
				if (eqSchedule.binarySchedule[t] != eqSchedule.binarySchedule[t - 1]) {
					//if schedule has changed, find new equilibrium

					//set all to 0 for this time
					for (size_t a = 0; a < Network.vertices; ++a) {
						for (size_t b = 0; b < Network.vertices; ++b) {
							eqSchedule.arcFlowAll[t][a][b] = 0.0;
						}
					}

					for (size_t od = 0; od < Network.numberODpairs; ++od)
						for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
							eqSchedule.pathFlow[t][od][r] = 0.0;
							eqSchedule.touristPathFlow[t][od][r] = 0.0;
						}

					//add the equilibrium flows (so needs all at 0)
					convexCombinations(eqSchedule, Network, 0.001, 0.0001, t);//convergenceCriterion,  epsilon for stepsize?

				}
				else {
					eqSchedule.arcFlowAll[t] = eqSchedule.arcFlowAll[t - 1];//so only arcFlowAll is completely accurate!
				}
			}
			currentEqCosts = costsSchedule(Network, Maintenance.T, eqSchedule.scheduledCapacities, eqSchedule.arcFlowAll);

			cout << currentEqCosts << "  ";
			if (currentEqCosts < bestEqCosts || bestEqCosts == 0) {
				bestEqCosts = currentEqCosts;
				bestAllEqSchedule = eqSchedule;
			}

		}
	}
	cout << "Lowest cost: " << costsSchedule(Network, Maintenance.T, bestAllEqSchedule.scheduledCapacities, bestAllEqSchedule.arcFlowAll) << ' ';
	ofstream bestAllEq(whichComputer + "bestAllEqBenchmarkB.txt");
	printRoutes(bestAllEq, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);
	printSchedule(bestAllEq, Maintenance.T, Maintenance.M, bestAllEqSchedule.binarySchedule);
	printTraffic(bestAllEq, Maintenance.T, Network.vertices, bestAllEqSchedule.arcFlowAll);
	
	bestAllEq << bestEqCosts; // costsSchedule(Network, Maintenance.T, bestAllEqSchedule.scheduledCapacities, bestAllEqSchedule.arcFlowAll);


	cout << "\nImplementation time: " << time.elapsed() << " seconds\n";
}