#include "ScheduleCheckFunctions.h"

#include <vector>
#include <cstddef>//for size_t

using namespace std;

void findInterruptedRoutes(size_t M, const vector<vector<size_t>> &locations, size_t ODpairs, const vector<size_t>& numberODpaths, const vector<vector<vector<size_t>>>& ODpaths, vector<vector<vector<size_t>>> &interruptedRoutes) {//set the routes interrupted for each maintenance activity

	vector<size_t>::iterator locInPath;

	//find which routes maintenance disturbs
	for (size_t m = 0; m < M; ++m) {
		for (size_t n = 0; n < locations[m].size(); n += 2) {
			for (size_t od = 0; od < ODpairs; ++od)
				for (size_t r = 0; r < numberODpaths[od]; ++r) {
					//go through all routes
					//for (size_t a = 0; a < ODpaths[od][r].size(); ++a) {
						//ODpaths[od][r][a]
					locInPath = find(ODpaths[od][r].begin(), ODpaths[od][r].end(), locations[m][n]);
					if (locInPath != ODpaths[od][r].end()) {
						//check both directions!!
					}

				}
		}
	}

	return;
}