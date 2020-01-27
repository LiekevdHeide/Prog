#include "MaintenanceActivities.h"

#include <iostream> //print on screen
#include <fstream> //for reading
#include <vector>
#include <string>
#include <cassert>//assertion of ifstream


using namespace std;
MaintenanceActivities::MaintenanceActivities(string maintenanceInput, size_t vertices, size_t ODpairs, vector<size_t> &numODroutes) {

	ifstream readFrom(maintenanceInput);
	assert(readFrom && "Wrong maintenance input doc");
	string trash;
	readFrom >> trash;//trash the first line

	readFrom >> M;
	readFrom >> T;

	vector<size_t> mDurations(M, 0);
	for (size_t m = 0; m < M; ++m) {
		readFrom >> mDurations[m];
	}
	duration = mDurations;

	vector<double> mCosts(M, 0.0);
	for (size_t m = 0; m < M; ++m) {
		readFrom >> mCosts[m];
	}
	costs = mCosts;

	//find all locations for all maintenance projects.
	size_t loc = 0;
	vector<vector<size_t>> perM(M, vector<size_t>());
	vector<size_t> allLocs;
	for (size_t m = 0; m < M; ++m) {
		//for number of locations per maintenance
		readFrom >> loc;
		while (loc < vertices) {
			allLocs.push_back(loc);
			readFrom >> loc;
		}
		perM[m] = allLocs;
		allLocs.clear();
	}
	locationSets = perM;

	//costReductions if combined maintenance
	vector<vector<double>> reductions(M, vector<double>(M, 0.0));
	for(size_t m = 0; m < M; ++m)
		for (size_t n = 0; n < M; ++n) {
			readFrom >> reductions[m][n];
		}
	costReduction = reductions;



	//initialize interruptedRoutes;
	vector<vector<vector<size_t>>> initInterruptedRoutes(M, vector<vector<size_t>>(ODpairs, vector<size_t>()));
	for(size_t m = 0; m < M; ++m)
		for (size_t od = 0; od < ODpairs; ++od) {
			cout << "r:" <<numODroutes[od]<< " ";
			for (size_t r = 0; r < numODroutes[od]; ++r) {
				cout << m << ":" << od << ":" << r << "  ";
				initInterruptedRoutes[m][od].push_back(0);
			}
		}

	interruptedRoutes = initInterruptedRoutes;
	cout << "Created maintenance projects\n";
}