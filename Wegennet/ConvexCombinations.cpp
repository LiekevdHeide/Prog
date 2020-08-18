#include "ScheduleAndFlows.h"
#include "ConvexCombinations.h"
#include "RoadNetwork.h"
#include "generalPrintingFunctions.h"

#include <iostream>
#include <cstddef> //necessery for size_t!!
#include <vector>

void convexCombinations(ScheduleAndFlows &Flow, RoadNetwork &Roads, double convergenceCriterion, double accuracyStepsize, size_t time) {
	//all path & arc flows are 0

	//initialize: for every OD-pair, assign all demand to the path with the lowest freeflow.
	size_t shortestPath{ 0 };
	double shortestLength{ 0.0 };
	for (size_t od = 0; od < Roads.numberODpairs; ++od) {
		//find shortest path
		shortestPath = findShortestPath(Roads, od, vector<vector<double>>(Roads.vertices, vector<double>(Roads.vertices, 0.0)), Flow.scheduledCapacities[time], Flow.numAvailableRoutes[time][od], Flow.availableRoutes[time][od], shortestLength);
		//assign all demand to shortest path & corresponding arcs
		Flow.addPathFlows(time, od, shortestPath, Roads.ODdemands[od], Roads.ODpaths[od][shortestPath]);
	}
	
	//start CC method:
	vector<vector<double>> directionPathFlow = Flow.pathFlow[time];
	vector<vector<double>> oldPathFlow = Flow.pathFlow[time];

	double convergenceTest = convergenceCriterion + 2;
	double alpha;//distance
	size_t counter = 0;
	vector<vector<double>> directionArcFlow(Roads.vertices, vector<double>(Roads.vertices, 0.0));
	vector<vector<double>> oldArcFlow = Flow.arcFlowAll[time];//vertices x vertices arc flows
	vector<vector<double>> newArcFlow = Flow.arcFlowAll[time];

	//ScheduleAndFlows newFlow(0, Roads.vertices, 0, Roads.numberODpairs, Roads.numberODpaths);

	while (convergenceTest > convergenceCriterion && counter < 1000 ) {//Flow.pathFlow is the old one, newPathFlow is the next direction
		//cout << "                 " << counter << '\n';
		//save old flows
		//oldPathFlow = Flow.pathFlow[0];
		oldArcFlow = newArcFlow;

		//direction finding (find new shortest path using the old flows to calculate the travel time)
		for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			shortestPath = findShortestPath(Roads, od, oldArcFlow, Flow.scheduledCapacities[time], Flow.numAvailableRoutes[time][od], Flow.availableRoutes[time][od], shortestLength);
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
		
		//step size
		alpha = findGoodStepsize(directionArcFlow, oldArcFlow, Roads, Flow.scheduledCapacities[time], accuracyStepsize);
		//alpha = 2.0 / (counter + 2.0);
		cout << "stepsize: " << alpha << '\n';
		//move to new flow using step size
		for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			for (size_t r = 0; r < Flow.numAvailableRoutes[time][od]; ++r) {
				size_t p = Flow.availableRoutes[time][od][r];//from index to actual route
				Flow.addPathFlows(time, od, p, alpha * (directionPathFlow[od][p] - Flow.pathFlow[time][od][p]), Roads.ODpaths[od][p]);//adjusts Flow.arcFLowAll and Flow.pathFlow
			}
		}

		//set newArcFLow to updated arcFlowAll (nec?)
		newArcFlow = Flow.arcFlowAll[time];
		
		//calculate convergenceTest
		convergenceTest = calculateConvergenceTest(oldArcFlow, newArcFlow, Roads, Flow.scheduledCapacities[time], Flow.numAvailableRoutes[time], Flow.availableRoutes[time]);
		++counter;//for trial
		
		//---------------------------------------------------------------
		//trail so print
		cout << "pTimes" << counter << ':' << convergenceTest << ' ';
		for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			for (size_t p = 0; p < Flow.numAvailableRoutes[time][od]; ++p) {
				cout << Roads.pathTravelTime(Roads.ODpaths[od][Flow.availableRoutes[time][od][p]], Flow.arcFlowAll[time], Flow.scheduledCapacities[time]) << 'f';
				cout << Flow.pathFlow[time][od][Flow.availableRoutes[time][od][p]] << ' ';
			}
			cout << '\n';
		}
	}
	cout << "count:" << counter << ' ';
	//Flow and Roads are changed and sent back

	return;
}
