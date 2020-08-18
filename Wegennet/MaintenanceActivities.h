#pragma once
#include <vector>
#include <string>

using namespace std;
class MaintenanceActivities
{

public:
	//constructor!
	MaintenanceActivities(string maintenanceInput, size_t vertices, size_t ODpairs, vector<size_t>& ODroutes);

	size_t M; //number of activities
	size_t T; //time horizon
	
	vector<size_t> duration;//size=M, duration per maintenance
	
	vector<vector<size_t>> locationSets;//size=M, numLocationsOfM, location set per maintenance activity

	vector<vector<vector<size_t>>> interruptedRoutes;//maintenance, OD pair, routes that are interrupted (binary)

	//NOT USED!:
	//matrix of reduction of costs if scheduled simultaneously
	//vector<vector<double>> costReduction;//size=MxM, reduction if 2 at the same time.
	//vector<double> costs;//costs per maintenance activity
	
};

