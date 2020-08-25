//checking
#include "performanceTimer.h"
#include "generalPrintingFunctions.h"
//classes
#include "RoadNetwork.h"
#include "MaintenanceActivities.h"
#include "ScheduleAndFlows.h"
#include "VNSparameters.h"

#include "ScheduleCheckFunctions.h"
#include "costCalculationFunctions.h"
#include "printingResults.h"

#include "ConvexCombinations.h"
#include "InitializationFunctions.h"
#include "PSAP.h"
#include "DepthFirstSearchFunctions.h"
#include "TouristAlternative.h"

#include "VNS.h"

#include <vector>
//#include <functional> //if want to make a standard function that can be different functions (such as policies etc)
//#include <random>
#include <iostream>
#include <fstream>
#include <cstddef> //necessery for size_t!!


using namespace std;

int main(int argc, char* argv[])
{
	//string whichComputer{ "X:/My Documents/Wegennetwerk/Experiment 2/" };
	//string whichComputer{ "C:/Users/Gebruiker/Documents/Wegennetwerk/Experiment trials/" };
	string whichComputer{ "/home/p279495/Wegennet/outputFiles/" };
	
	//read the data file with road network    //should be inputs: road network configurations (set of vertices, arcs (directed), OD-pairs, constant for traffic time per arc)
	string roadInput = argv[1]; // whichComputer + "NetworkInputTrials1.txt";
	RoadNetwork Network(roadInput);
	//find all routes
	depthFirstSearch(Network);

	//read data file with maintenance action info    //should be inputs: data for maintenance projects (location sets, durations, reduction of cap, time frame)
	string maintenanceInput = argv[2];// whichComputer + "maintenanceInputTrials1.txt";
	MaintenanceActivities Maintenance(maintenanceInput, Network.vertices, Network.numberODpairs, Network.numberODpaths);
	
	string VNSparametersInput = argv[3]; // whichComputer + "VNSinputTrial.txt";
	VNSparameters VNSpars(VNSparametersInput);


	Timer time;
	
	for (size_t odDemands = 0; odDemands < 2; ++odDemands) {
		for (size_t odPairs = 0; odPairs < Network.numberODpairs; ++odPairs) {
			if (odDemands == 0) {
				Network.ODdemands[odPairs] = 10;
			}
			else {
				Network.ODdemands[odPairs] = 20;
			}
		}

		for (size_t capBC = 0; capBC < 10; ++capBC) {
			switch (capBC) {
			case 0:
				Network.standardCapacities[1][2] = 5;
				Network.freeFlowTimes[1][2] = 1;
				Network.standardCapacities[2][1] = 5;
				Network.freeFlowTimes[2][1] = 1;
			case 1:
				Network.standardCapacities[1][2] = 10;
				Network.freeFlowTimes[1][2] = 1;
				Network.standardCapacities[2][1] = 10;
				Network.freeFlowTimes[2][1] = 1;
			case 2:
				Network.standardCapacities[1][2] = 15;
				Network.freeFlowTimes[1][2] = 1;
				Network.standardCapacities[2][1] = 15;
				Network.freeFlowTimes[2][1] = 1;
			case 3:
				Network.standardCapacities[1][2] = 5;
				Network.freeFlowTimes[1][2] = 5;
				Network.standardCapacities[2][1] = 5;
				Network.freeFlowTimes[2][1] = 5;
			case 4:
				Network.standardCapacities[1][2] = 10;
				Network.freeFlowTimes[1][2] = 5;
				Network.standardCapacities[2][1] = 10;
				Network.freeFlowTimes[2][1] = 5;
			case 5:
				Network.standardCapacities[1][2] = 15;
				Network.freeFlowTimes[1][2] = 5;
				Network.standardCapacities[2][1] = 15;
				Network.freeFlowTimes[2][1] = 5;
			case 6:
				Network.standardCapacities[1][2] = 5;
				Network.freeFlowTimes[1][2] = 10;
				Network.standardCapacities[2][1] = 5;
				Network.freeFlowTimes[2][1] = 10;
			case 7:
				Network.standardCapacities[1][2] = 10;
				Network.freeFlowTimes[1][2] = 10;
				Network.standardCapacities[2][1] = 10;
				Network.freeFlowTimes[2][1] = 10;
			case 8:
				Network.standardCapacities[1][2] = 15;
				Network.freeFlowTimes[1][2] = 10;
				Network.standardCapacities[2][1] = 15;
				Network.freeFlowTimes[2][1] = 10;
			case 9:
				Network.standardCapacities[1][2] = 5;
				Network.freeFlowTimes[1][2] = 1;
				Network.standardCapacities[2][1] = 10;
				Network.freeFlowTimes[2][1] = 10;
			}

			ofstream allResults(whichComputer + "overallResults.txt", ios::app); 

			allResults << Maintenance.M << ' ' << Maintenance.T << ' ' << Maintenance.runOutPeriod << ' ' << Network.vertices << ' ' << Network.touristPercentage << ' ' << Network.ODdemands[0] << ' ';
			for (size_t v = 0; v < Network.vertices; ++v)
				for (size_t w = 0; w < Network.vertices; ++w) {
					allResults << Network.freeFlowTimes[v][w] << ' ' << Network.standardCapacities[v][w] << ' ';
				}


			//find which maintenance interrupts which route:
			findInterruptedRoutes(Maintenance.M, Maintenance.locationSets, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, Maintenance.interruptedRoutes);
			cout << '\n';
			for (size_t m = 0; m < Maintenance.M; ++m) {
				print2Dim(Maintenance.interruptedRoutes[m], Network.numberODpairs);//
			}

			//class? with schedule / capacities per link per time unit?
			ScheduleAndFlows Schedule(Maintenance.T, Network.vertices, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.standardCapacities);

			//print2Dim(Schedule.binarySchedule, Maintenance.T, Maintenance.M);

			//write results to:
			//ofstream write(whichComputer + "ResultsAllEnum1.txt");  //, std::ios::app for adding to end of file
			//write << roadInput << ' ' << maintenanceInput << '\n';
			//printRoutes(write, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);

			cout << "Reading time: " << time.elapsed() << " seconds \n";
			time.reset();

			//calculate equilibrium with no maintenance scheduled.
			//CONVEX COMBINATIONS:
			ScheduleAndFlows equilibrium(2, Network.vertices, 0, Network.numberODpairs, Network.numberODpaths, Network.standardCapacities);//all flows 0, no maintenance
			convexCombinations(equilibrium, Network, 0.001, 0.0001, 0);//convergenceCriterion,  epsilon for stepsize?, time!

			//find alternative routes uninformed using cplex / IP problem, returns arcFlows for alternative tourists per M combi: //flows for: s(2^M), v, v
			cout << "EqTravelTimes\n";
			vector<vector<double>>eqTravelTimeArcs(Network.vertices, vector<double>(Network.vertices, 0.0));
			for (size_t i = 0; i < Network.vertices; ++i) {
				for (size_t j = 0; j < Network.vertices; ++j) {
					if (Network.standardCapacities[i][j] > 0) {
						eqTravelTimeArcs[i][j] = Network.travelTimeRoad(Network.standardCapacities[i][j], Network.freeFlowTimes[i][j], equilibrium.arcFlowAll[0][i][j]);
					}
					cout << eqTravelTimeArcs[i][j] << ' ';
				}
				cout << '\n';
			}

			//NECESSARY PARAMETERS!!
			size_t numSmallStep = 1;

			double bigCost = 10000000;
			vector<vector<vector<double>>> touristAlternativeFlowsPerwholeState = touristAlternative(Network, Network.muCostUninformed, Maintenance.M, Maintenance.locationSets, equilibrium.pathFlow[0], eqTravelTimeArcs);
			//double maxTimeVNS = VNSpars.maxTime;
			//size_t maxIterationsVNS = VNSpars.maxIterations;

			//Make initial schedule solution 
			cout << "--------------Create initial schedule ----------------\n";
			initializeSchedule(Schedule, equilibrium, Maintenance, Network, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
			cout << costFromStarttimes(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << '\n';

			//implement VNS
			string bestHeursol = whichComputer + "BestHeurSolution" + '_' + to_string(Maintenance.duration[0]) + '_'+ to_string(capBC) + "comparison" + ".txt";

			VNS(Network, Schedule, Maintenance, VNSpars, equilibrium, touristAlternativeFlowsPerwholeState, bestHeursol, numSmallStep, bigCost);
			size_t currentM = 0;
			for (size_t m = 0; m < Maintenance.M; ++m) {
				allResults << Schedule.startTimes[m] << ' ';
			}

			allResults << costFromSchedule(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << ' ' << time.elapsed() << ' ';
			//cout << "\nComplete enumeration:\n";
			time.reset();

			//-----------------------------------------------------------------------------------------------------------
			//Complete enumeration init

			double currentCosts = 0.0;
			double bestCosts = 0.0;

			ScheduleAndFlows bestSchedule(Schedule);

			//brute force:					
			size_t totalSchedule = pow(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M);
			for (size_t s = 1; s < totalSchedule; ++s) { // s < time periods ^ maintenance activities = pow(Maint.T, Maint.M) //pow(Maintenance.T, Maintenance.M)
			//for(size_t s = 181260; s < 189980; ++s){											 //adjust schedule
				if (s % 10000 == 0) {
					cout << s << ':';
				}
				if (bruteForceSchedule(Maintenance, s, Schedule.binarySchedule)) {//start from t = 1 (t = 0 is equilibrium!) (returns if schedule is feasible wrt time + Schedule.binarySchedule)		
					//initialize the start for  pathFlows for informed

					//start at equilibrium. 
					//(not necessary?)
					for (size_t od = 0; od < Network.numberODpairs; ++od)
						for (size_t p = 0; p < Network.numberODpaths[od]; ++p) {
							Schedule.pathFlow[0][od][p] = equilibrium.pathFlow[0][od][p] * (1.00 - Network.touristPercentage);
						}

					currentCosts = costFromSchedule(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);//updates capacities, availableRoutes, touristFlows, recurrentFlows + calculates costs

					//test
					for (size_t t = 0; t < Maintenance.T; ++t) {
						//print2Dim(Schedule.pathFlow[t]);
						for (size_t od = 0; od < Network.numberODpairs; ++od)
							for (size_t r = 0; r < Network.numberODpaths[od]; ++r) {
								if (Schedule.pathFlow[t][od][r] < -0.0000000000000001) {
									cout << "NEGFLOW" << Schedule.pathFlow[t][od][r] << ' ';
								}
							}

					}

					if (currentCosts < bestCosts || bestCosts == 0) {
						bestCosts = currentCosts;
						bestSchedule = Schedule;
					}

					//cost function: return total travel time + joint costs of maintenance

				}

			}


			//-----------------------------------------------------------------------------------------------------------------

			ofstream bestSolution(whichComputer + "BestSolution" + '_' + to_string(Network.ODdemands[0]) + '_' + to_string(capBC) + "comparison" + ".txt");
			bestSolution << roadInput << '_' << maintenanceInput << '\n';
			bestSolution << "T M OD\n";
			bestSolution << Maintenance.T << ' ' << Maintenance.M << ' ' << Network.numberODpairs << '\n';
			for (size_t od = 0; od < Network.numberODpairs; ++od) {
				bestSolution << "pathsOD" << od;
			}
			bestSolution << '\n';
			for (size_t od = 0; od < Network.numberODpairs; ++od) {
				bestSolution << Network.numberODpaths[od] << ' ';
			}
			bestSolution << '\n';
			printSchedule(bestSolution, Maintenance.T, Maintenance.M, bestSchedule.binarySchedule);
			printRoutes(bestSolution, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);

			printTraffic(bestSolution, Maintenance.T, Network.vertices, bestSchedule.arcFlowAll);
			printRecurringTraffic(bestSolution, Maintenance.T, Network.numberODpairs, Network.numberODpaths, bestSchedule.pathFlow);
			bestSolution << costFromSchedule(Network, Maintenance, bestSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);


			//generalresultsoutput
			for (size_t allM = 0; allM < Maintenance.M; ++allM) {
				allResults << bestSchedule.startTimes[allM] << ' ';
			}
			allResults << totalTravelTime(Network, Maintenance.T, bestSchedule.scheduledCapacities, bestSchedule.arcFlowAll) << ' ';
			allResults << time.elapsed() << ' ';
			time.reset();

			cout << "\nImplementation time: " << time.elapsed() << " seconds\n";
			//write << "\n runtime: " << time.elapsed() << " seconds\n";
			bestSolution.close();
			//worstSolution.close();

	//	}  

	//}

//----------------------------------------------------------------
//get benchmark: flows always in equilibrium (for best solution)
/*
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
					//equilibriumBenchmark.touristPathFlow[t][od][r] = 0.0;
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
*/
//---------------------------------------------------------------
//find the new equilibrium for all maintenance combination's capacities

			vector<vector<vector<double>>> eqFlowsPerWholestate(pow(2, Maintenance.M), vector<vector<double>>(Network.vertices, vector<double>(Network.vertices, 0.0)));

			for (size_t s = 0; s < pow(2, Maintenance.M); ++s) {
				ScheduleAndFlows eqIteration(1, Network.vertices, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.standardCapacities);//all flows 0, no maintenance

				wholeScheduleToMaintenance(2, Maintenance.M, s, eqIteration.binarySchedule[0]); // get binary vector of scheduled maintenance activities

				//need to reset eqSchedule.scheduledCapacities??
				binaryToCapacities(1, Maintenance.M, eqIteration.binarySchedule, Maintenance.locationSets, Network.standardCapacities, eqIteration.scheduledCapacities);

				if (adjustAvailableRoutes(1, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, eqIteration.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, eqIteration.availableRoutes, eqIteration.numAvailableRoutes)) {
					//use convexCombi to get equilibrium flows
					cout << "here";
					convexCombinations(eqIteration, Network, 0.001, 0.0001, 0);//check this... + if it uses actualCapacities!!
					eqFlowsPerWholestate[s] = eqIteration.arcFlowAll[0];
					cout << totalTravelTime(Network, 1, eqIteration.scheduledCapacities, eqIteration.arcFlowAll) << ' ';
				}

			}
			//-------------------------
			//brute force opt for allEqual
			double currentEqCosts = 0.0;
			double bestEqCosts = 0.0;
			cout << "\n Best all equilibrium schedule:\n";
			ScheduleAndFlows eqSchedule(Schedule);
			ScheduleAndFlows bestEqSchedule(Schedule);
			cout << costFromSchedule(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << ' ';

			//for all new schedules?
			for (size_t s = 0; s < pow(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M); s++) { // s < time periods ^ maintenance activities = pow(Maint.T, Maint.M) //
				//adjust schedule
				wholeScheduleToMaintenance(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M, s, eqSchedule.startTimes);
				if (bruteForceSchedule(Maintenance, s, eqSchedule.binarySchedule)) {//start from t = 1 (t = 0 is equilibrium!) (also adjustsavailableRoute)
					eqSchedule.arcFlowAll[0] = equilibrium.arcFlowAll[0];
					eqSchedule.pathFlow[0] = equilibrium.pathFlow[0];
					//adjust scheduled capacities
					binaryToCapacities(Maintenance.T, Maintenance.M, eqSchedule.binarySchedule, Maintenance.locationSets, Network.standardCapacities, eqSchedule.scheduledCapacities);

					//check feasibility
					if (adjustAvailableRoutes(Maintenance.T, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, eqSchedule.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, eqSchedule.availableRoutes, eqSchedule.numAvailableRoutes)) {

						//add the eqFlowsPerWholestate for the correct wholeState to arcFLowAll!
						adjustTouristArcFLows(Maintenance.T, Maintenance.M, eqSchedule.binarySchedule, eqFlowsPerWholestate, eqSchedule.arcFlowAll);

						//Check!! if scheduledCapacities is changed...
						currentEqCosts = totalTravelTime(Network, Maintenance.T, eqSchedule.scheduledCapacities, eqSchedule.arcFlowAll);
						if (currentEqCosts < bestEqCosts || bestEqCosts == 0) {
							bestEqCosts = currentEqCosts;
							bestEqSchedule = eqSchedule;
							for (size_t m = 0; m < Maintenance.M; ++m) {
								cout << eqSchedule.startTimes[m] << '-';
							}
							cout << ' ';
							Schedule.startTimes = eqSchedule.startTimes;
							cout << currentEqCosts << ' ';
						}
					}
				}
			}
			cout << "Lowest cost: " << totalTravelTime(Network, Maintenance.T, bestEqSchedule.scheduledCapacities, bestEqSchedule.arcFlowAll) << ':';
			for (size_t m = 0; m < Maintenance.M; ++m) {
				cout << bestEqSchedule.startTimes[m] << '-';
			}
			ofstream bestAllEq(whichComputer + "bestAllEq_" + to_string(Network.ODdemands[0]) + to_string(capBC) + "comparison.txt");
			printRoutes(bestAllEq, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);
			printSchedule(bestAllEq, Maintenance.T, Maintenance.M, bestEqSchedule.binarySchedule);
			printTraffic(bestAllEq, Maintenance.T, Network.vertices, bestEqSchedule.arcFlowAll);
			cout << " actualCost " << costFromSchedule(Network, Maintenance, bestEqSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << ' '; //changes the flows!!
			bestAllEq << bestEqCosts; // costsSchedule(Network, Maintenance.T, bestAllEqSchedule.scheduledCapacities, bestAllEqSchedule.arcFlowAll);
			printTraffic(bestAllEq, Maintenance.T, Network.vertices, bestEqSchedule.arcFlowAll);


			for (size_t allM = 0; allM < Maintenance.M; ++allM) {
				allResults << bestEqSchedule.startTimes[allM] << ' ';
			}
			allResults << bestEqCosts << ' ' << costFromSchedule(Network, Maintenance, bestEqSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << ' ';
			allResults << time.elapsed() << '\n';
			time.reset();

			allResults.close();
		}
	}
	
	
}