#pragma once
#include <vector>

using namespace std;
class MaintenanceActivities
{

public:
	//constructor!
	MaintenanceActivities() {};

	size_t M; //number of activities
	
	vector<size_t> duration;//size=M, duration per maintenance
	
	vector<vector<size_t>> locationSets;//size=M, numLocationsOfM, location set per maintenance activity
	
	vector<double> costs;//costs per maintenance activity
	//matrix of reduction of costs if scheduled simultaneously
	vector<vector<double>> costReduction;//size=MxM, reduction if 2 at the same time.


};

