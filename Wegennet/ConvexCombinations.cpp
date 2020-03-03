#include "ScheduleAndFlows.h"
#include "ConvexCombinations.h"
#include "RoadNetwork.h"
#include "generalPrintingFunctions.h"

#include <iostream>

void convexCombinations(ScheduleAndFlows &Flow, RoadNetwork &Roads, double convergenceCriterion, double accuracyStepsize, size_t time) {
	//all path & arc flows are 0

	//initialize: for every OD-pair, assign all demand to the path with the lowest freeflow.
	size_t shortestPath{ 0 };
	for (size_t od = 0; od < Roads.numberODpairs; ++od) {
		//find shortest path
		shortestPath = findShortestPath(Roads, od, vector<vector<double>>(Roads.vertices, vector<double>(Roads.vertices, 0.0)), Flow.scheduledCapacities[time], Flow.numAvailableRoutes[time][od], Flow.availableRoutes[time][od]);
		//assign all demand to shortest path & corresponding arcs
		Flow.addPathFlows(time, od, shortestPath, Roads.ODdemands[od], Roads.ODpaths[od][shortestPath]);
	}
	/*
	cout << "\n path flows:\n";
	print2Dim(Flow.pathFlow[time], Roads.numberODpairs);
	cout << "arc flows:\n";
	print2Dim(Flow.arcFlow[time][0], Roads.vertices);
	*/

	//start CC method:
	vector<vector<double>> directionPathFlow = Flow.pathFlow[time];
	vector<vector<double>> oldPathFlow = Flow.pathFlow[time];

	double convergenceTest = convergenceCriterion + 2;
	double alpha = 0.3;//distance
	size_t counter = 0;
	vector<vector<double>> directionArcFlow(Roads.vertices, vector<double>(Roads.vertices, 0.0));
	vector<vector<double>> oldArcFlow(Roads.vertices, vector<double>(Roads.vertices, 0.0));
	//sum over OD pairs
	for (size_t a = 0; a < Roads.vertices; ++a)
		for (size_t b = 0; b < Roads.vertices; ++b)
			for (size_t od = 0; od < Roads.numberODpairs; ++od) {
				oldArcFlow[a][b] += Flow.arcFlow[time][od][a][b];
			}
	vector<vector<double>> newArcFlow(Roads.vertices, vector<double>(Roads.vertices, 0.0));
	newArcFlow = oldArcFlow;
	//ScheduleAndFlows newFlow(0, Roads.vertices, 0, Roads.numberODpairs, Roads.numberODpaths);

	while (convergenceTest > convergenceCriterion && counter < 20 ) {//Flow.pathFlow is the old one, newPathFlow is the next direction
		//cout << "                 " << counter << '\n';
		//save old flows
		//oldPathFlow = Flow.pathFlow[0];
		oldArcFlow = newArcFlow;

		//direction finding (find new shortest path using the old flows to calculate the travel time)
		for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			shortestPath = findShortestPath(Roads, od, oldArcFlow, Flow.scheduledCapacities[time], Flow.numAvailableRoutes[time][od], Flow.availableRoutes[time][od]);
			//set all to 0
			for (size_t p = 0; p < Flow.numAvailableRoutes[time][od]; ++p) {
				directionPathFlow[od][Flow.availableRoutes[time][od][p]] = 0;
			}
			//get new path flows
			directionPathFlow[od][shortestPath] = Roads.ODdemands[od];
		}
		//cout << "Direction path flow:\n";
		//print2Dim(directionPathFlow, Roads.numberODpairs);

		//line search to find step size: Bolzano search/bisection method p.86 Sheffi)
			//get arc flows (sum over OD pairs)
		for(size_t a = 0; a < Roads.vertices; ++a)
			for (size_t b = 0; b < Roads.vertices; ++b) {
				directionArcFlow[a][b] = 0;
			}
		for (size_t od = 0; od < Roads.numberODpairs; ++od)
			for (size_t r = 0; r < Flow.numAvailableRoutes[time][od]; ++r) {
				size_t p = Flow.availableRoutes[time][od][r]; //from index to actual route 
				for(size_t a = 0; a < Roads.ODpaths[od][p].size() - 1; ++a){ //
					directionArcFlow[Roads.ODpaths[od][p][a]][Roads.ODpaths[od][p][a + 1]] += directionPathFlow[od][p];
				}
			}
		
		/*cout << "direction flows:\n";
		print2Dim(directionArcFlow, Roads.vertices);
		cout << "old flows:\n";
		print2Dim(oldArcFlow, Roads.vertices);
		*/
		alpha = findGoodStepsize(directionArcFlow, oldArcFlow, Roads, Flow.scheduledCapacities[time], accuracyStepsize);
		//if (alpha < accuracyStepsize) {
		//	break;
		//}
		//move to new flow using step size
		for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			for (size_t r = 0; r < Flow.numAvailableRoutes[time][od]; ++r) {
				size_t p = Flow.availableRoutes[time][od][r];//from index to actual route
				Flow.addPathFlows(time, od, p, alpha * (directionPathFlow[od][p] - Flow.pathFlow[time][od][p]), Roads.ODpaths[od][p]);
			}
		}

			//sum over OD pairs
		for (size_t a = 0; a < Roads.vertices; ++a)
			for (size_t b = 0; b < Roads.vertices; ++b) {
				newArcFlow[a][b] = 0.0;
				for (size_t od = 0; od < Roads.numberODpairs; ++od) {
					newArcFlow[a][b] += Flow.arcFlow[time][od][a][b];
				}
			}

		//cout << "New pathflows:\n";
		//print2Dim(Flow.pathFlow[time], Roads.numberODpairs);
		//calculate convergenceTest
		
		convergenceTest = calculateConvergenceTest(oldArcFlow, newArcFlow, Roads, Flow.scheduledCapacities[time], Flow.numAvailableRoutes[time], Flow.availableRoutes[time]);
		++counter;//for trial

		/*cout << convergenceTest << ' ';
		cout << "\nTravel times per path:\n";
		for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			for (size_t r = 0; r < Flow.numAvailableRoutes[time][od]; ++r) {
				size_t p = Flow.availableRoutes[time][od][r];//index to path
				if (Flow.pathFlow[time][od][p] > 0) {
					cout << Roads.pathTravelTime(Roads.ODpaths[od][p], newArcFlow) << ' ';
				}
				else {
					cout << "-- ";
				}
			}
			cout << '\n';
		}
		*/
	}

	//Flow and Roads are changed and sent back
	/*cout << "            Final equilirbium solution:\n";
	cout << "Flows per path:\n";
	print2Dim(Flow.pathFlow[time], Roads.numberODpairs);
	cout << "Travel times per path:\n";
	for (size_t od = 0; od < Roads.numberODpairs; ++od) {
		for (size_t r = 0; r < Flow.numAvailableRoutes[time][od]; ++r) {
			size_t p = Flow.availableRoutes[time][od][r];
			if (Flow.pathFlow[time][od][p] > 0) {
				cout << Roads.pathTravelTime(Roads.ODpaths[od][p], newArcFlow) << ' ';
			}
			else {
				cout << "-- ";
			}
		}
		cout << '\n';
	}
	*/

	return;
}
