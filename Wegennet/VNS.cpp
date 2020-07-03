#include "VNS.h"
#include "generalPrintingFunctions.h"
#include "printingResults.h"
#include "costCalculationFunctions.h"

#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"
#include "InitializationFunctions.h"
#include "MaintenanceActivities.h"

#include <vector>

using namespace std;

void VNS(RoadNetwork &Network, ScheduleAndFlows &Schedule, MaintenanceActivities &Maintenance,  ScheduleAndFlows &equilibrium, vector<vector<vector<double>>> &touristAlternativeFlowsPerwholeState, string resultPrint, size_t runOutPeriod, size_t numSmallStep, double bigCost, double maxTime, size_t maxIterations) {
	//ofstream VNSPerformance(VNSperformancePrint);
	ofstream bestHeur(resultPrint);
	size_t NoImprovementsFor = 0;
	size_t noOverallImprovements = 0;

	double bestHeurCosts = costFromStarttimes(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
	double overallBestHeurCosts = bestHeurCosts;
	ScheduleAndFlows bestHeurSchedule = Schedule;
	ScheduleAndFlows overallBestHeurSchedule = Schedule;
	
	while (noOverallImprovements < 500) {
		//printSchedule(VNSPerformance, Maintenance.T, Maintenance.M, Schedule.startTimes);
		size_t iteration = 0;
		//bestHeurSchedule = Schedule;
		double improvedCosts;
		size_t neighbourhood = 0;
		if (NoImprovementsFor > 80) {
			//cout << "restart\n";
			NoImprovementsFor = 0;
			initializeSchedule(Schedule, equilibrium, Maintenance, Network, touristAlternativeFlowsPerwholeState, numSmallStep, runOutPeriod, bigCost);
			bestHeurCosts = costFromSchedule(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
			//VNSPerformance << "--------------restart-----------------------\n";
		}
		while (iteration < maxIterations && neighbourhood < 2) { //Maintenance.M  actualNoImprovementsFor < 50
			//cout << "NBH" << neighbourhood << ' ';
			iteration++;

			if (neighbourhood == 0) {
				//shake + adjust startTimes
				shakeTimeNBH(Maintenance, Schedule, runOutPeriod);//only adjusts startTimes
				//local search in timeMove
				improvedCosts = timeMoveNeighbourhood(Network, Schedule, Maintenance, touristAlternativeFlowsPerwholeState, numSmallStep, runOutPeriod, bigCost);

			}
			if (neighbourhood == 1) {//swap neighbourhood
				//shake
				shakeSwapNBH(Maintenance, Schedule);//only adjusts startTimes
				//local search in swap neighbourhood
				improvedCosts = swapNeighbourhood(Network, Schedule, Maintenance, touristAlternativeFlowsPerwholeState, numSmallStep, runOutPeriod, bigCost);

			}
			//VNSPerformance << "it" << iteration << ' ' << costFromSchedule(Network, Maintenance, Schedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost);
			//printSchedule(VNSPerformance, Maintenance.T, Maintenance.M, Schedule.binarySchedule);

		
			//cout << "\nImprovement after shake:" << improvedCosts << '\n';

			++noOverallImprovements;
			if (improvedCosts < bestHeurCosts) {
				cout << "\nNew best:" << improvedCosts << " OLD:" << bestHeurCosts << '\n';
				bestHeurSchedule = Schedule;
				bestHeurCosts = improvedCosts;
				if (improvedCosts < overallBestHeurCosts) {
					overallBestHeurSchedule = Schedule;
					overallBestHeurCosts = improvedCosts;
					noOverallImprovements = 0;
				}
				NoImprovementsFor = 0;
				neighbourhood = 0;
			}
			else {
				++NoImprovementsFor;
				++neighbourhood;
				Schedule = bestHeurSchedule;
			}

		}

		//VNSPerformance << "\n--------------------------------------------------------------------------------\n";


	}
	bestHeur << "bestHeur results";//roadInput << '_' << maintenanceInput << '\n';
	bestHeur << "T M OD\n";
	bestHeur << Maintenance.T << ' ' << Maintenance.M << ' ' << Network.numberODpairs << '\n';
	for (size_t od = 0; od < Network.numberODpairs; ++od) {
		bestHeur << "pathsOD" << od;
	}
	bestHeur << '\n';
	for (size_t od = 0; od < Network.numberODpairs; ++od) {
		bestHeur << Network.numberODpaths[od] << ' ';
	}
	bestHeur << '\n';
	bestHeur << costFromStarttimes(Network, Maintenance, overallBestHeurSchedule, touristAlternativeFlowsPerwholeState, numSmallStep, bigCost) << '\n';
	printSchedule(bestHeur, Maintenance.T, Maintenance.M, overallBestHeurSchedule.binarySchedule);
	print2Dim(overallBestHeurSchedule.binarySchedule, Maintenance.T, bestHeur);
	printRoutes(bestHeur, Network.numberODpairs, Network.numberODpaths, Network.ODpaths);

	printTraffic(bestHeur, Maintenance.T, Network.vertices, overallBestHeurSchedule.arcFlowAll);
	printRecurringTraffic(bestHeur, Maintenance.T, Network.numberODpairs, Network.numberODpaths, overallBestHeurSchedule.pathFlow);
	bestHeur << "Tourist arc flows:\n";
	printTraffic(bestHeur, Maintenance.T, Network.vertices, overallBestHeurSchedule.arcFlowTourist);
	printCostsPerTime(bestHeur, Network, Maintenance.T, overallBestHeurSchedule.scheduledCapacities, overallBestHeurSchedule.arcFlowAll);

	bestHeur.close();
	//VNSPerformance.close();
	Schedule = overallBestHeurSchedule;

	return;
}