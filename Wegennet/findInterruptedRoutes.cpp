#include "ScheduleCheckFunctions.h"

#include <vector>
#include <iostream>
#include <cstddef>//for size_t

using namespace std;

void findInterruptedRoutes(size_t M, const vector<vector<size_t>> &locations, size_t ODpairs, const vector<size_t>& numberODpaths, const vector<vector<vector<size_t>>>& ODpaths, vector<vector<vector<size_t>>> &interruptedRoutes) {//set the routes interrupted for each maintenance activity
	//interruptedroutes: m, od, r (bianry)
	//simpler?
	for (size_t m = 0; m < M; ++m) {
		for (size_t n = 0; n < locations[m].size(); n+=2) {
			for (size_t od = 0; od < ODpairs; ++od) {
				for (size_t p = 0; p < numberODpaths[od]; ++p) {
					for (size_t i = 0; i < ODpaths[od][p].size() - 1; ++i) {
						if (ODpaths[od][p][i] == locations[m][n] && ODpaths[od][p][i + 1] == locations[m][n + 1]) {
							interruptedRoutes[m][od][p] = 1;
							cout << "m" << m << "od" << od << "p" << p << ' ';
							break;
						}
					}
				}
			}
		}
	}

	/*old method:
	vector<size_t>::const_iterator locInPath;
	size_t locationP;
	bool routeUnavailable = false;
	//find which routes maintenance disturbs
	for (size_t m = 0; m < M; ++m) {
		for (size_t n = 0; n < locations[m].size(); n += 2) {
			for (size_t od = 0; od < ODpairs; ++od)
				for (size_t r = 0; r < numberODpaths[od]; ++r) {//go through all routes
					routeUnavailable = false;
						
					locInPath = find(ODpaths[od][r].begin(), ODpaths[od][r].end(), locations[m][n]);
					if (locInPath != ODpaths[od][r].end()) {//start location found
						//check both directions!!
						locationP = distance(ODpaths[od][r].begin(), locInPath);
						if (locationP > 0) {
							if (locations[m][n + 1] == ODpaths[od][r][locationP - 1]) {
								//route unavailable.
								routeUnavailable = true;
							}
						}
						if (locationP < ODpaths[od][r].size() - 1 && routeUnavailable == false) {
							if (locations[m][n + 1] == ODpaths[od][r][locationP + 1]) {
								//route unavailable
								routeUnavailable = true;
							}
						}
					}

					if (routeUnavailable) {
						//save this route as when this maintenance is scheduled.
						interruptedRoutes[m][od][r] = 1;
						//cout << "INTERRUPTED "<< interruptedRoutes[m][od][r] << ' ';
					}
					
				}
		}
	}
	*/
	return;
}