#include "ScheduleCheckFunctions.h"
#include <vector>

using namespace std;

void adjustAvailableRoutes(size_t T, size_t M, size_t ODpairs, vector<size_t> &numberODpaths, vector<vector<vector<size_t>>> &ODpaths, const std::vector< std::vector<size_t>>& binarySchedule, const std::vector<std::vector<size_t>>& locations, std::vector<std::vector<std::vector<size_t>>>& availableRoutes, std::vector<std::vector<size_t>>& numAvailableRoutes){

	//find which routes maintenance disturbs
	for (size_t m = 0; m < M; ++m) {
		locations; //M, location sets
		ODpaths;//OD, path, order of vertices

		for (size_t od = 0; od < ODpairs; ++od)
		for (size_t r = 0; r < numberODpaths[od]; ++r) {
			//go through all routes

		}
	}


	for(size_t t=0; t < T; ++t)
		for (size_t od = 0; od < ODpairs; ++od) {
			
			//check in binary schedule when each is implemented
			//use vector of which route is unavailable bc of this maintenance




			numAvailableRoutes[t][od] = availableRoutes[t][od].size();
		}


	return;
}