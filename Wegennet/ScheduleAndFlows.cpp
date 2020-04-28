#include "ScheduleAndFlows.h"

using namespace std;
ScheduleAndFlows::ScheduleAndFlows(size_t T, size_t vertices, size_t M, size_t ODpairs, std::vector<size_t> &numberODpaths, std::vector<std::vector<double>> &standardCapacities) {//constructor

	//initialize arc flows
	
	vector<vector<vector<double>>> aAll(T, vector<vector<double>>(vertices, vector<double>(vertices, 0.0)));
	arcFlowAll = aAll;
	arcFlowTourist = aAll;

	//initialize path flows
	vector<vector<vector<double>>> pFlows(T, vector<vector<double>>(ODpairs, vector<double>()));
	for(size_t t=0; t < T; ++t)
	for (size_t OD = 0; OD < ODpairs; ++OD) {
		for (size_t p = 0; p < numberODpaths[OD]; ++p) {
			pFlows[t][OD].push_back(0.0);
		}
	}
	pathFlow = pFlows;
	//touristPathFlow = pFlows;

	//initialize schedule
	vector<vector<size_t>> schedule(T, vector<size_t>(M, 0));
	binarySchedule = schedule;

	vector<vector<vector<double>>> caps(T, vector<vector<double>>(vertices, vector<double>(vertices, 0.0)));
	for (size_t t = 0; t < T; ++t)
		caps[t] = standardCapacities;
	
	scheduledCapacities = caps;

	std::vector<std::vector<std::vector<size_t>>> constructAvailableRoutes(T, vector<vector<size_t>>(ODpairs, vector<size_t>()));
	for(size_t t=0; t < T; ++t)
		for(size_t od = 0; od < ODpairs; ++od)
			for (size_t p = 0; p < numberODpaths[od]; ++p) {
				constructAvailableRoutes[t][od].push_back(p);
			}
	availableRoutes = constructAvailableRoutes;

	vector<vector<size_t>> constructNumAvailableRoutes(T, vector<size_t>(ODpairs, 1));
	for(size_t t = 0; t < T; ++t)
		for (size_t od = 0; od < ODpairs; ++od) {
			constructNumAvailableRoutes[t][od] = numberODpaths[od];
		}

	numAvailableRoutes = constructNumAvailableRoutes;
}
