#pragma once
#include <vector>
class ScheduleAndFlows
{

public:
	//schedule of maintenance projects
	std::vector<std::vector<size_t>> binarySchedule;//time, activities planned (binary)
	std::vector<std::vector<std::vector<double>>> scheduledCapacities;//time, capacities

	//arc flows
	std::vector<std::vector< std::vector<std::vector<double>>>> arcFlow; //time, ODpair, vertice a, vertice b

	//path flows
	std::vector< std::vector<std::vector<double>>> pathFlow; //time, OD pair, path

	ScheduleAndFlows(size_t T, size_t vertices, size_t M, size_t ODpairs, std::vector<size_t> &numberODpaths, std::vector<std::vector<double>>& standardCapacities);

	void addPathFlows(size_t time, size_t OD, size_t path, double flow, std::vector<size_t> arcsForPath) {
		pathFlow[time][OD][path] += flow;
		for (size_t a = 0; a < arcsForPath.size() - 1; ++a) {
			arcFlow[time][OD][arcsForPath[a]][arcsForPath[a + 1]] += flow;
		}
	}
	

};

