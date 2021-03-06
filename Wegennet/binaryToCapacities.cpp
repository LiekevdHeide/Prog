#include "ScheduleCheckFunctions.h"
#include<vector>
#include <cstddef> //necessery for size_t!!

using namespace std;
void binaryToCapacities(size_t T, size_t M, const std::vector< std::vector<size_t>>& binarySchedule, const vector<vector<size_t>>& locations, std::vector<std::vector<double>> &standardCap, std::vector< std::vector< std::vector<double>>> &resultingCapacities) {

	for (size_t t = 0; t < T; ++t) {
		resultingCapacities[t] = standardCap;
		for (size_t m = 0; m < M; ++m) {
			if (binarySchedule[t][m] > 0) {
				for (size_t l = 0; l < locations[m].size(); l+=2) {
					resultingCapacities[t][locations[m][l]][locations[m][l + 1]] = 0.0;// *= 0.05;
				}
			}
		}

	}
		

	return;
}