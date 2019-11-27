#include "ScheduleAndFlows.h"
#include "ConvexCombinations.h"
#include "RoadNetwork.h"
#include "generalPrintingFunctions.h"

#include <iostream>

void convexCombinations(ScheduleAndFlows &Flow, RoadNetwork &Roads, double convergenceCriterion) {
	//all path & arc flows are 0

	//initialize: for every OD-pair, assign all demand to the path with the lowest freeflow.
	
	size_t shortestPath{ 0 };//(Roads.numberODpairs, 0);
	for (size_t od = 0; od < Roads.numberODpairs; ++od) {
		//find shortest path
		shortestPath = findShortestPath(Roads, od, vector<double>(Roads.numberODpaths[od], 0.0));
		//assign all demand to shortest path & corresponding arcs
		Flow.addPathFlows(0, od, shortestPath, Roads.ODdemands[od], Roads.ODpaths[od][shortestPath]);
	}
	cout << "\n path flows:\n";
	print2Dim(Flow.pathFlow[0], 1);
	cout << "arc flows:\n";
	print2Dim(Flow.arcFlow[0][0], Roads.vertices);

	//start CC method:
	//vector<size_t> shortestPathNew(Roads.numberODpairs, 0);
	//vector<vector<vector<double>>> oldarcFlows = Flow.arcFlow[0];
	vector<vector<double>> newPathFlow = Flow.pathFlow[0];

	double convergenceTest = convergenceCriterion - 2;
	double alpha = 0.5;//distance
	size_t counter = 0;
	while (convergenceTest <= convergenceCriterion && counter < 2) {//Flow.pathFlow is the old one, newPathFlow is the next direction
		//update travelCosts (not nec, just add the flows..?)

		//direction finding (find new shortest path using the old flows to calculate the travel time)
		for (size_t od = 0; od < Roads.numberODpairs; ++od) {
			shortestPath = findShortestPath(Roads, od, Flow.pathFlow[0][od]);
			for (size_t p = 0; p < Roads.numberODpaths[od]; ++p) {
				newPathFlow[od][p] = 0;
			}
			//get new path flows
			newPathFlow[od][shortestPath] = Roads.ODdemands[od];
		}

		//line search to find step size!


		//move to new flow using step size
		for (size_t od = 0; od < Roads.numberODpairs; ++od)
			for (size_t p = 0; p < Roads.numberODpaths[od]; ++p)
				Flow.addPathFlows(0, od, p,   alpha * (newPathFlow[od][p] - Flow.pathFlow[0][od][p]) , Roads.ODpaths[od][p]);

		print2Dim(Flow.pathFlow[0], 1);
		//calculate convergenceTest
		
		convergenceTest = calculateConvergenceTest(newPathFlow, Flow.pathFlow[0], Roads.numberODpairs, Roads.numberODpaths);
		cout << convergenceTest << ' ';
		++counter;//for trial
		cout << "CCstep\n";
	}

	return;
}
