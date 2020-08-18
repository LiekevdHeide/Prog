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


using namespace std;

int main()
{
	//string whichComputer{ "X:/My Documents/Wegennetwerk/Experiment 2/" };
	string whichComputer{ "C:/Users/Gebruiker/Documents/Wegennetwerk/Experiment trials/" };
	//read the data file with road network    //should be inputs: road network configurations (set of vertices, arcs (directed), OD-pairs, constant for traffic time per arc)
	string roadInput = whichComputer + "NetworkInputTrials1.txt";
	RoadNetwork Network(roadInput);
	//find all routes
	depthFirstSearch(Network);

	//read data file with maintenance action info    //should be inputs: data for maintenance projects (location sets, durations, reduction of cap, time frame)
	string maintenanceInput = whichComputer + "maintenanceInputTrials1.txt";
	MaintenanceActivities Maintenance(maintenanceInput, Network.vertices, Network.numberODpairs, Network.numberODpaths);
	
	string VNSparametersInput = whichComputer + "VNSinputTrial.txt";
	VNSparameters VNSpars(VNSparametersInput);


	Timer time;
	ofstream allResults(whichComputer + "maintenanceTimecomparison.txt"); //std::ios::app
	allResults << "#step dem opt bruteM m0 m1 m2 m3 m4 m5 Hsched0 Hsched1 Hsched2 Hsched3 Hsched4 Hsched5 Hcost Bsched0 Bsched1 Bsched2 Bsched3 Bsched4 Bsched5 Bcost time\n";
	//allResults.close();

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
					string bestHeursol = whichComputer + "BestHeurSolution" + to_string(Maintenance.duration[0]) + "comparison" + ".txt";

					VNS(Network, Schedule, Maintenance, VNSpars, equilibrium, touristAlternativeFlowsPerwholeState, bestHeursol, numSmallStep, bigCost);
					size_t currentM = 0;
					for (size_t m = 0; m < Maintenance.M; ++m) {
							allResults << Schedule.startTimes[m] << ' ';
					}

					allResults << costFromSchedule(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << ' ';
					//cout << "\nComplete enumeration:\n";

					//Complete enumeration init
					double currentCosts = 0.0;
					double bestCosts = 0.0;
					
					ScheduleAndFlows bestSchedule(Schedule);
					

					//-----------------------------------------------------------------------------------------------------------
					//brute force: !!add shift to one!!
					/*
					size_t totalSchedule = pow(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M);
					for (size_t s = 1; s < totalSchedule; ++s) { // s < time periods ^ maintenance activities = pow(Maint.T, Maint.M) //pow(Maintenance.T, Maintenance.M)
					//for(size_t s = 181260; s < 189980; ++s){											 //adjust schedule
						if (s % 1000 == 0) {
							cout << s << ':';
						}
						if (bruteForceSchedule(Maintenance, s, Maintenance.runOutPeriod, Schedule.binarySchedule)) {//start from t = 1 (t = 0 is equilibrium!) (returns if schedule is feasible wrt time + Schedule.binarySchedule)		
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
							//print stuff
							//currentCosts = totalTravelTime(Network, Maintenance.T, Schedule.scheduledCapacities, Schedule.arcFlowAll);

							//cout << currentCosts << "\n";
							//write << s << ' ' << currentCosts << '\n';
							//printSchedule(write, Maintenance.T, Maintenance.M, Schedule.binarySchedule);
							//write << '\n';
							//write << "All arc flows:\n";
							//printTraffic(write, Maintenance.T, Network.vertices, Schedule.arcFlowAll);
							//write << "Tourist arc flows:\n";
							//printTraffic(write, Maintenance.T, Network.vertices, Schedule.arcFlowTourist);
							//printRecurringTraffic(write, Maintenance.T, Network.numberODpairs, Network.numberODpaths, Schedule.pathFlow);
							//write << '\n';




							//write << "++++++++++++++++++++++++++++\n";

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
							//cout << ' ';// "IMPOSSIBLE:" << s << ' ';

						}

					}
					*/
					//-----------------------------------------------------------------------------------------------------------------

					ofstream bestSolution(whichComputer + "BestSolution" + '_' + to_string(Network.ODdemands[0]) + '_' + to_string(Maintenance.duration[0]) + "comparison" + ".txt");
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
					currentM = 0;
					for (size_t allM = 0; allM < Maintenance.M; ++allM) {
							allResults << bestSchedule.startTimes[currentM] << ' ';
					}
					allResults << totalTravelTime(Network, Maintenance.T, bestSchedule.scheduledCapacities, bestSchedule.arcFlowAll) << ' ';
					allResults << time.elapsed() << '\n';

					cout << "\nImplementation time: " << time.elapsed() << " seconds\n";
					//write << "\n runtime: " << time.elapsed() << " seconds\n";
					bestSolution.close();
					//worstSolution.close();

			//	}
				allResults.close();
			//}
		
	//allResults.close();
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
							//eqSchedule.touristPathFlow[t][od][r] = 0.0;
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

	*/
	
}