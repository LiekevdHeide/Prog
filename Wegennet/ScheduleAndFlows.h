#pragma once
#include <vector>
#include <iostream>
class ScheduleAndFlows
{

public:
	//schedule of maintenance projects
	std::vector<std::vector<size_t>> binarySchedule;//time, activities planned (binary)
	std::vector<std::vector<std::vector<double>>> scheduledCapacities;//time, capacities

	std::vector<std::vector<std::vector<size_t>>> availableRoutes;//t, OD, routeNum
	std::vector<std::vector<size_t>> numAvailableRoutes;//t, OD

	//arc flows
	std::vector<std::vector< std::vector<std::vector<double>>>> arcFlow; //time, ODpair, vertice a, vertice b

	std::vector<std::vector< std::vector<double>>> arcFlowAll; //time, vertice a, vertice b

	//path flows
	std::vector< std::vector<std::vector<double>>> pathFlow; //time, OD pair, path

	ScheduleAndFlows(size_t T, size_t vertices, size_t M, size_t ODpairs, std::vector<size_t> &numberODpaths, std::vector<std::vector<double>>& standardCapacities);

	void addPathFlows(size_t time, size_t OD, size_t path, double flow, std::vector<size_t> arcsForPath) {
		pathFlow[time][OD][path] += flow;
		for (size_t a = 0; a < arcsForPath.size() - 1; ++a) {
			arcFlow[time][OD][arcsForPath[a]][arcsForPath[a + 1]] += flow;
			arcFlowAll[time][arcsForPath[a]][arcsForPath[a + 1]] += flow;
		}
	}
	
	void addArcFlowAll(size_t time, size_t numPairs, const std::vector<size_t > &numPaths, const std::vector< std::vector< std::vector<size_t>>> &paths) {
		//ASSUMES ARCFLOW at time is 0!!
		for(size_t od = 0; od < numPairs; ++od)
			for (size_t p = 0; p < numPaths[od]; ++p) 
				for (size_t a = 0; a < paths[od][p].size() - 1; a++){
					//std::cout << od << p << a << " " << paths[od][p][a] << ' ' << paths[od][p][a + 1] << '  ';
					arcFlowAll[time][paths[od][p][a]][paths[od][p][a + 1]] += pathFlow[time][od][p];
				}
	}

};

