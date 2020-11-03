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
#include <iomanip> //necessery for setprecision


using namespace std;

int main(int argc, char* argv[])
{
	//string whichComputer{ "X:/My Documents/Wegennetwerk/Experiment 2/" };
	string whichComputer{ "C:/Users/Gebruiker/Documents/Wegennetwerk/Experiment trials/" };
	//string whichComputer{ "/home/p279495/Wegennet/outputFiles/Exp180920" };
	string outputNames = whichComputer;

	//read the data file with road network    //should be inputs: road network configurations (set of vertices, arcs (directed), OD-pairs, constant for traffic time per arc)
	string roadInput = argv[1]; // whichComputer + "NetworkInputTrials1.txt";
	RoadNetwork Network(roadInput);
	//find all routes
	depthFirstSearch(Network);

	//read data file with maintenance action info    //should be inputs: data for maintenance projects (location sets, durations, reduction of cap, time frame)
	string maintenanceInput = argv[2];// whichComputer + "maintenanceInputTrials1.txt";
	MaintenanceActivities Maintenance(maintenanceInput, Network.vertices, Network.numberODpairs, Network.numberODpaths);
	//Maintenance.T -= Maintenance.runOutPeriod;
	  //Maintenance.runOutPeriod = 0; 
	string VNSparametersInput = argv[3]; // whichComputer + "VNSinputTrial.txt";
	VNSparameters VNSpars(VNSparametersInput);

	cout << fixed << setprecision(10);
	Timer time;

	outputNames += to_string(Maintenance.M) + '_' + to_string(Maintenance.T) + '_' + to_string(Maintenance.runOutPeriod) + '_';
	bool isUsed = false;
	vector<size_t> locationOptions{ 0, 1, 0, 2, 1, 2, 1, 3, 2, 3 };
	vector<size_t> locationToMaintenance(Maintenance.M, 0);
	for (size_t m = 0; m < 5; ++m) {
		for (size_t n = 0; n < Maintenance.M; ++n) {
			if (Maintenance.locationSets[n][0] == locationOptions[2 * m] && Maintenance.locationSets[n][1] == locationOptions[2 * m + 1]) {
				locationToMaintenance[n] = m;
				isUsed = true;
				outputNames += to_string(m);
				break;
			}
		}
		isUsed = false;
	}

	outputNames += '_';
	//add durations
	for (size_t m = 0; m < Maintenance.M; ++m) {
		outputNames += to_string(Maintenance.duration[m]);
	}
	//network part of name
	outputNames += 'N';
	outputNames += to_string(Network.vertices) + '_' + to_string(Network.touristPercentage).substr(0, 3) + '_' + to_string(Network.muCostUninformed).substr(0, 3) + '_' + to_string(Network.ODdemands[0]).substr(0, 2) + '_' + to_string(Network.freeFlowTimes[1][2]).substr(0, 2) + '_' + to_string(Network.freeFlowTimes[2][3]).substr(0, 2);



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

	double bigCost = 10000000000;
	vector<vector<vector<double>>> touristAlternativeFlowsPerwholeState = touristAlternative(Network, Network.muCostUninformed, Maintenance.M, Maintenance.locationSets, equilibrium.pathFlow[0], eqTravelTimeArcs);
	//double maxTimeVNS = VNSpars.maxTime;
	//size_t maxIterationsVNS = VNSpars.maxIterations;

	//Make initial schedule solution 
	cout << "--------------Create initial schedule ----------------\n";
	initializeSchedule(Schedule, equilibrium, Maintenance, Network, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
	for (size_t m = 0; m < Maintenance.M; ++m) {
		cout << Schedule.startTimes[m] << ' ';
	}
	cout << costFromStarttimes(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << '\n';

	//implement VNS
	string bestHeursol = outputNames + "BestHeurSolution.txt";

	//VNS(Network, Schedule, Maintenance, VNSpars, equilibrium, touristAlternativeFlowsPerwholeState, bestHeursol, numSmallStep, bigCost);

	//save stuff for overall results
	vector<size_t> printHeurStarttimes = vector<size_t>(5, 0);
	for (size_t m = 0; m < Maintenance.M; ++m) {
		printHeurStarttimes[locationToMaintenance[m]] = Schedule.startTimes[m];
	}
	double printHeurcosts = costFromSchedule(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
	double printHeurtime = time.elapsed();

	//find overlap:
	double totalDuration = 0;
	double timeMaintenanceIsPlanned = 0;
	for (size_t m = 0; m < Maintenance.M; ++m) {
		totalDuration += Maintenance.duration[m];
	}

	for (size_t t = 0; t < Maintenance.T - Maintenance.runOutPeriod; ++t) {
		for (size_t m = 0; m < Maintenance.M; ++m) {
			if (Schedule.binarySchedule[t][m] > 0) {
				++timeMaintenanceIsPlanned;
				break;
			}
		}
	}
	double printHeurOverlap = 1 - (timeMaintenanceIsPlanned / totalDuration);

	double finalMaintenanceEnds = 0;
	for (size_t m = 0; m < Maintenance.M; ++m) {
		if (Schedule.startTimes[m] + Maintenance.duration[m] > finalMaintenanceEnds) {
			finalMaintenanceEnds = Schedule.startTimes[m] + Maintenance.duration[m];
		}
	}

	double printHeurSpreiding = finalMaintenanceEnds - timeMaintenanceIsPlanned;
	//cout << "\nComplete enumeration:\n";
	time.reset();

	//-----------------------------------------------------------------------------------------------------------
	//Complete enumeration init
	
	double currentCosts = 0.0;
	double bestCosts = 0.0;

	ScheduleAndFlows bestSchedule(Schedule);
	vector<size_t> optimalSchedules;
	//brute force:					
	size_t totalSchedule = pow(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M);
	for (size_t s = 1; s < totalSchedule; ++s) { // s < time periods ^ maintenance activities = pow(Maint.T, Maint.M) //pow(Maintenance.T, Maintenance.M)
	//for(size_t s = 181260; s < 189980; ++s){											 //adjust schedule
		if (s % 50000 == 0) {
			cout << s << ':';
		}
		if (bruteForceSchedule(Maintenance, s, Schedule.startTimes, Schedule.binarySchedule)) {//start from t = 1 (t = 0 is equilibrium!) (returns if schedule is feasible wrt time + Schedule.binarySchedule)		
			//initialize the start for  pathFlows for informed

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

			if (currentCosts <= (bestCosts + 0.0000000000001)) {
				optimalSchedules.push_back(s);
			}
			if (currentCosts < bestCosts || bestCosts == 0) {
				bestCosts = currentCosts;
				bestSchedule = Schedule;
				optimalSchedules = vector<size_t>(1 , s); 
			}
			

			//cost function: return total travel time + joint costs of maintenance

		}

	}
	cout << "Optimal schedules: ";

	for (size_t a = 0; a < optimalSchedules.size(); ++a) {
		cout << optimalSchedules[a] << ": ";
		vector<size_t> stTimes(Maintenance.M, 0);
		wholeScheduleToMaintenance(2, Maintenance.M, optimalSchedules[a], stTimes);
		for (size_t m = 0; m < Maintenance.M; ++m) {
			cout << stTimes[m] << ' ';
		}
		cout << '\n';

	}
	cout << '\n';
	//-----------------------------------------------------------------------------------------------------------------

	//print optimal schedule to results file (same as heur results file)
	ofstream bestSolution(outputNames + "BestSolution.txt");
	bestSolution << fixed << setprecision(10);
	bestSolution << "optimal solution " << roadInput << '_' << maintenanceInput << '\n';
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
	bestSolution << costFromSchedule(Network, Maintenance, bestSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
	printSchedule(bestSolution, Maintenance.T, Maintenance.M, bestSchedule.binarySchedule);
	print2Dim(bestSchedule.binarySchedule, Maintenance.T, bestSolution);
	printRoutes(bestSolution, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);

	printTraffic(bestSolution, Maintenance.T, Network.vertices, bestSchedule.arcFlowAll);
	printRecurringTraffic(bestSolution, Maintenance.T, Network.numberODpairs, Network.numberODpaths, bestSchedule.pathFlow);
	bestSolution << "Tourist arc flows:\n";
	printTraffic(bestSolution, Maintenance.T, Network.vertices, bestSchedule.arcFlowTourist);
	printCostsPerTime(bestSolution, Network, Maintenance.T, bestSchedule.scheduledCapacities, bestSchedule.arcFlowAll);
	bestSolution.close();

	//generalresultsoutput
	vector<size_t> printOptStarttimes(5, 0);
	for (size_t m = 0; m < Maintenance.M; ++m) {
		printOptStarttimes[locationToMaintenance[m]] = bestSchedule.startTimes[m];
	}
	double printOptcosts = totalTravelTime(Network, Maintenance.T, bestSchedule.scheduledCapacities, bestSchedule.arcFlowAll);
	double printOpttime = time.elapsed();
	time.reset();

	//find overlap:
	timeMaintenanceIsPlanned = 0;
	for (size_t t = 0; t < Maintenance.T - Maintenance.runOutPeriod; ++t) {
		for (size_t m = 0; m < Maintenance.M; ++m) {
			if (bestSchedule.binarySchedule[t][m] > 0) {
				++timeMaintenanceIsPlanned;
				break;
			}
		}
	}
	double printOptOverlap = 1 - (timeMaintenanceIsPlanned / totalDuration); //totalDuration remains the same
   //find spreiding:
	finalMaintenanceEnds = 0;
	for (size_t m = 0; m < Maintenance.M; ++m) {
		if (bestSchedule.startTimes[m] + Maintenance.duration[m] > finalMaintenanceEnds) {
			finalMaintenanceEnds = bestSchedule.startTimes[m] + Maintenance.duration[m];
		}
	}

	double printOptSpreiding = finalMaintenanceEnds - timeMaintenanceIsPlanned;

	cout << "\nImplementation time: " << time.elapsed() << " seconds\n";
	
	//-------------------------------------------------------------------------------------------------
	//find the new equilibrium for all maintenance combination's capacities

	vector<vector<vector<double>>> eqFlowsPerWholestate(pow(2, Maintenance.M), vector<vector<double>>(Network.vertices, vector<double>(Network.vertices, 0.0)));
	//ofstream printConvexCombi("C:/Users/Gebruiker/Documents/Wegennetwerk/Experiment trials/convexCombi.txt");
	for (size_t s = 0; s < pow(2, Maintenance.M); ++s) {
		ScheduleAndFlows eqIteration(1, Network.vertices, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.standardCapacities);//all flows 0, no maintenance

		wholeScheduleToMaintenance(2, Maintenance.M, s, eqIteration.binarySchedule[0]); // get binary vector of scheduled maintenance activities

		//need to reset eqSchedule.scheduledCapacities??
		binaryToCapacities(1, Maintenance.M, eqIteration.binarySchedule, Maintenance.locationSets, Network.standardCapacities, eqIteration.scheduledCapacities);

		if (adjustAvailableRoutes(1, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, eqIteration.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, eqIteration.availableRoutes, eqIteration.numAvailableRoutes)) {
			//use convexCombi to get equilibrium flows
			//cout << "here";
			convexCombinations(eqIteration, Network, 0.001, 0.0001, 0);//check this... + if it uses actualCapacities!!
			eqFlowsPerWholestate[s] = eqIteration.arcFlowAll[0];
			//for (size_t m = 0; m < Maintenance.M; ++m) {
			//	printConvexCombi << eqIteration.binarySchedule[0][m] << ' ';
			//}
			//printConvexCombi << totalTravelTime(Network, 1, eqIteration.scheduledCapacities, eqIteration.arcFlowAll) << ' ';
			//printTraffic(printConvexCombi, 1, Network.vertices, eqIteration.arcFlowAll);
		}

	}
	//printConvexCombi.close();
	//-------------------------
	//brute force opt for allEqual
	double currentEqCosts = 0.0;
	double bestEqCosts = 0.0;
	vector<size_t> bestWholeStates;
	cout << "\n Best all equilibrium schedule:\n";
	ScheduleAndFlows eqSchedule(Schedule);
	//ScheduleAndFlows bestEqSchedule(Schedule);
	cout << costFromSchedule(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << ' ';

	//for all new schedules?
  //ofstream trialWrite(whichComputer +"trialWrite.txt");
  //trialWrite << fixed << setprecision(10);
	for (size_t s = 0; s < pow(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M); s++) { // s < time periods ^ maintenance activities = pow(Maint.T, Maint.M) //
		//adjust schedule
		wholeScheduleToMaintenance(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M, s, eqSchedule.startTimes);
		if (bruteForceSchedule(Maintenance, s, eqSchedule.startTimes, eqSchedule.binarySchedule)) {//start from t = 1 (t = 0 is equilibrium!) (also adjustsavailableRoute)

			//adjust scheduled capacities
			binaryToCapacities(Maintenance.T, Maintenance.M, eqSchedule.binarySchedule, Maintenance.locationSets, Network.standardCapacities, eqSchedule.scheduledCapacities);

			//check feasibility
			if (adjustAvailableRoutes(Maintenance.T, Maintenance.M, Network.numberODpairs, Network.numberODpaths, Network.ODpaths, eqSchedule.binarySchedule, Maintenance.locationSets, Maintenance.interruptedRoutes, eqSchedule.availableRoutes, eqSchedule.numAvailableRoutes)) {

				//add the eqFlowsPerWholestate for the correct wholeState to arcFLowAll!
				adjustTouristArcFLows(Maintenance.T, Maintenance.M, eqSchedule.binarySchedule, eqFlowsPerWholestate, eqSchedule.arcFlowAll);

				//Check!! if scheduledCapacities is changed...
				currentEqCosts = totalTravelTime(Network, Maintenance.T, eqSchedule.scheduledCapacities, eqSchedule.arcFlowAll);
				//if(currentEqCosts < (bestEqCosts + 0.001) || bestEqCosts == 0){
				  //for(size_t m = 0; m < Maintenance.M; ++m){
				  //  trialWrite << eqSchedule.startTimes[m] << ' ';
				  //}
				  //trialWrite << currentEqCosts << ' ' << s <<  '\n';
				//}
				if (currentEqCosts <= (bestEqCosts + 0.0000000000001)) {
					bestWholeStates.push_back(s);
					cout << bestEqCosts << ' ';
				}
				else if (currentEqCosts < (bestEqCosts - 0.0000000000001) || bestEqCosts == 0) {
					cout << currentEqCosts - bestEqCosts << ' ';
					bestEqCosts = currentEqCosts;
					//bestEqSchedule = eqSchedule;
					for (size_t m = 0; m < Maintenance.M; ++m) {
						cout << eqSchedule.startTimes[m] << '-';
					}
					cout << ' ';
					//Schedule.startTimes = eqSchedule.startTimes;
					cout << currentEqCosts << ' ';
					bestWholeStates = vector<size_t>(1, s);
				}
			}
		}
	}
	//trialWrite << "WhichBest:\n";
	cout << "Lowest cost: " << bestEqCosts;// totalTravelTime(Network, Maintenance.T, bestEqSchedule.scheduledCapacities, bestEqSchedule.arcFlowAll) << ':';
	cout << "set of best eq schedules: ";
	for (size_t s = 0; s < bestWholeStates.size(); ++s) {
		cout << bestWholeStates[s] << ' ';
		wholeScheduleToMaintenance(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M, bestWholeStates[s], eqSchedule.startTimes);
		for (size_t m = 0; m < Maintenance.M; ++ m) {
			cout << eqSchedule.startTimes[m] << '-';
		}
		cout << ' ';
	}
	cout << '\n';
	//use the vector of schedules with best eq cost to find the mean / min / max actual cost
	double eqActMin = 0;
	double eqActMax = 0;
	double eqActMean = 0;
	double eqActEarly = 0;
	double eqActcurrentCost = 0;

	size_t sumStartTimes = 0;
	size_t currentSumStartTimes = 0;
	vector<size_t> eqActMinStarttimes;
	vector<size_t> eqActMaxStarttimes;
	vector<size_t> eqActEarliestStarttimes;
	for (size_t state = 0; state < bestWholeStates.size(); ++state) {
		wholeScheduleToMaintenance(Maintenance.T - Maintenance.runOutPeriod, Maintenance.M, bestWholeStates[state], eqSchedule.startTimes);
		//check if only startTimes are used..
		eqActcurrentCost = costFromStarttimes(Network, Maintenance, eqSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
		//trialWrite << bestWholeStates[state] << ' ';
		//for(size_t m = 0; m < Maintenance.M; ++m){
		  //trialWrite << eqSchedule.startTimes[m] << ' ';
		//}
		//trialWrite << eqActcurrentCost << '\n';
		eqActMean += eqActcurrentCost;
		if (eqActcurrentCost < eqActMin || state == 0) {
			eqActMin = eqActcurrentCost;
			eqActMinStarttimes = eqSchedule.startTimes;

		}
		if (eqActcurrentCost > eqActMax || state == 0) {
			eqActMax = eqActcurrentCost;
			eqActMaxStarttimes = eqSchedule.startTimes;
		}
		for (size_t m = 0; m < Maintenance.M; ++m) {
			currentSumStartTimes += eqSchedule.startTimes[m];
		}
		if (currentSumStartTimes < sumStartTimes || sumStartTimes == 0) {
			eqActEarliestStarttimes = eqSchedule.startTimes;
			sumStartTimes = currentSumStartTimes;
			eqActEarly = eqActcurrentCost;
		}
		currentSumStartTimes = 0;

	}
	eqActMean /= bestWholeStates.size();
	//trialWrite.close();

	  //change eqSchedule to the schedule with lowest eq cost that results in lowest actual cost 
	eqSchedule.startTimes = eqActMinStarttimes;
	//set correct binary schedule
	for (size_t m = 0; m < Maintenance.M; ++m)
		for (size_t t = 0; t < Maintenance.T; ++t) {
			if (t < eqSchedule.startTimes[m] || t >= eqSchedule.startTimes[m] + Maintenance.duration[m]) {
				eqSchedule.binarySchedule[t][m] = 0;
			}
			else {
				eqSchedule.binarySchedule[t][m] = 1;
			}
		}
	//paste the correct flows for the corr wholeState
	adjustTouristArcFLows(Maintenance.T, Maintenance.M, eqSchedule.binarySchedule, eqFlowsPerWholestate, eqSchedule.arcFlowAll);
	//need to adjust capacities if we use it to calc cost...

	ofstream bestAllEq(outputNames + "bestAllEq.txt");
	bestAllEq << fixed << setprecision(10);
	printRoutes(bestAllEq, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);
	printSchedule(bestAllEq, Maintenance.T, Maintenance.M, eqSchedule.binarySchedule);
	printTraffic(bestAllEq, Maintenance.T, Network.vertices, eqSchedule.arcFlowAll);
	cout << "lowest actualCost " << costFromSchedule(Network, Maintenance, eqSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << ' '; //changes the flows!!
	bestAllEq << bestEqCosts << ' ';
	bestAllEq << "min actual cost schedule:\n";
	bestAllEq << costFromSchedule(Network, Maintenance, eqSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << '\n';
	printTraffic(bestAllEq, Maintenance.T, Network.vertices, eqSchedule.arcFlowAll);
	printRecurringTraffic(bestAllEq, Maintenance.T, Network.numberODpairs, Network.numberODpaths, eqSchedule.pathFlow);
	//max cost eq schedule:
	bestAllEq << "max actual cost schedule:\n";
	eqSchedule.startTimes = eqActMaxStarttimes;
	bestAllEq << costFromSchedule(Network, Maintenance, eqSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << '\n';
	printTraffic(bestAllEq, Maintenance.T, Network.vertices, eqSchedule.arcFlowAll);
	printRecurringTraffic(bestAllEq, Maintenance.T, Network.numberODpairs, Network.numberODpaths, eqSchedule.pathFlow);
	//earliest eq schedule:
	bestAllEq << "Earliest schedule: \n";
	eqSchedule.startTimes = eqActEarliestStarttimes;
	bestAllEq << costFromSchedule(Network, Maintenance, eqSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << '\n';
	printTraffic(bestAllEq, Maintenance.T, Network.vertices, eqSchedule.arcFlowAll);
	printRecurringTraffic(bestAllEq, Maintenance.T, Network.numberODpairs, Network.numberODpaths, eqSchedule.pathFlow);
	bestAllEq.close();

	vector<size_t> printEqStarttimes(5, 0);
	vector<size_t> printEqMaxStarttimes(5, 0);
	vector<size_t> printEqEarlyStarttimes(5, 0);
	for (size_t m = 0; m < Maintenance.M; ++m) {
		printEqStarttimes[locationToMaintenance[m]] = eqActMinStarttimes[m];
		printEqMaxStarttimes[locationToMaintenance[m]] = eqActMaxStarttimes[m];
		printEqEarlyStarttimes[locationToMaintenance[m]] = eqActEarliestStarttimes[m];
	}

	//find overlap:
	timeMaintenanceIsPlanned = 0;
	for (size_t t = 0; t < Maintenance.T - Maintenance.runOutPeriod; ++t) {
		for (size_t m = 0; m < Maintenance.M; ++m) {
			if (t >= eqActMinStarttimes[m] && t < eqActMinStarttimes[m] + Maintenance.duration[m]) {
				++timeMaintenanceIsPlanned;
				break;
			}
		}
	}
	double printEqOverlap = 1 - (timeMaintenanceIsPlanned / totalDuration);
	double printEqtime = time.elapsed();
	//---------------------------print overall results------------------------------------------------------

	//print specs for this instance
	ofstream allResults(whichComputer + "overallResults.txt", ios::app);
	allResults << fixed << setprecision(10);
	allResults << argv[1] << ' ' << argv[2] << ' ' << argv[3] << ' ' << Maintenance.M << ' ' << Maintenance.T << ' ' << Maintenance.runOutPeriod << ' ' << Network.vertices << ' ' << Network.muCostUninformed << ' ' << Network.touristPercentage << ' ' << Network.ODdemands[0] << ' ';
	for (size_t v = 0; v < Network.vertices; ++v)
		for (size_t w = 0; w < Network.vertices; ++w) {
			allResults << Network.freeFlowTimes[v][w] << ' ' << Network.standardCapacities[v][w] << ' ';
		}

	for (size_t m = 0; m < 5; ++m) {
		for (size_t n = 0; n < Maintenance.M; ++n) {
			if (Maintenance.locationSets[n][0] == locationOptions[2 * m] && Maintenance.locationSets[n][1] == locationOptions[2 * m + 1]) {
				locationToMaintenance[n] = m;
				allResults << 1 << ' ';
				isUsed = true;
				whichComputer += to_string(m);
				break;
			}
		}
		if (!isUsed) {
			allResults << 0 << ' ';
		}
		isUsed = false;
	}

	vector<size_t> toPrint(5, 0);
	for (size_t m = 0; m < Maintenance.M; ++m) {
		toPrint[locationToMaintenance[m]] = Maintenance.duration[m];
	}
	for (size_t m = 0; m < 5; ++m) {
		allResults << to_string(toPrint[m]) << ' ';
	}


	//print bestHeur
	for (size_t m = 0; m < 5; ++m) {
		allResults << printHeurStarttimes[m] << ' ';
	}
	allResults << printHeurcosts << ' ' << printHeurtime << ' ' << printHeurOverlap << ' ' << printHeurSpreiding << ' ';


	//print bestOpt
	/*for (size_t m = 0; m < 5; ++m) {
		allResults << printOptStarttimes[m] << ' ';
	}
	allResults << printOptcosts << ' ' << printOpttime << ' ' << printOptOverlap << ' ' << printOptSpreiding << ' ';
	*/
	//print bestEq + 2xcosts
	for (size_t m = 0; m < 5; ++m) {
		allResults << printEqStarttimes[m] << ' ';
	}
	allResults << bestEqCosts << ' ' << eqActMin << ' ' << printEqOverlap << ' ';
	for (size_t m = 0; m < 5; ++m) {
		allResults << printEqMaxStarttimes[m] << ' ';
	}
	allResults << eqActMax << ' ' << eqActMean << ' ';
	for (size_t m = 0; m < 5; ++m) {
		allResults << printEqEarlyStarttimes[m] << ' ';
	}
	allResults << eqActEarly << ' ';

	allResults << printEqtime << '\n';

	cout << " end";
	allResults.close();

	costFromSchedule(Network, Maintenance, bestSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
}