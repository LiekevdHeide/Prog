#include "ScheduleAndFlows.h"

using namespace std;
ScheduleAndFlows::ScheduleAndFlows(size_t T, size_t vertices, size_t M, size_t ODpairs, std::vector<size_t> &numberODpaths, std::vector<std::vector<double>> &standardCapacities) {//constructor

	//initialize arc flows
	vector<vector<vector<vector<double>>>> aFlows(T, vector<vector<vector<double>>>(ODpairs, vector<vector<double>>(vertices, vector<double>(vertices, 0.0))));
	arcFlow = aFlows;

	vector<vector<vector<double>>> aAll(T, vector<vector<double>>(vertices, vector<double>(vertices, 0.0)));
	arcFlowAll = aAll;

	//initialize path flows
	vector<vector<vector<double>>> pFlows(T, vector<vector<double>>(ODpairs, vector<double>()));
	for(size_t t=0; t < T; ++t)
	for (size_t OD = 0; OD < ODpairs; ++OD) {
		for (size_t p = 0; p < numberODpaths[OD]; ++p) {
			pFlows[t][OD].push_back(0.0);
		}
	}
	pathFlow = pFlows;

	//initialize schedule
	vector<vector<size_t>> schedule(T, vector<size_t>(M, 0));
	binarySchedule = schedule;

	vector<vector<vector<double>>> caps(T, vector<vector<double>>(vertices, vector<double>(vertices, 0.0)));
	for (size_t t = 0; t < T; ++t)
		caps[t] = standardCapacities;
	
	scheduledCapacities = caps;
}
