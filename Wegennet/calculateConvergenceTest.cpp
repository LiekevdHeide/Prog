#include "ConvexCombinations.h"

#include <vector>
using namespace std;
double calculateConvergenceTest(vector<vector<double>> xOld, vector<vector<double>> xNew, size_t OD, vector<size_t> ODpaths) {
	double max = 0.0;
	double test = 0.0;
	for (size_t od = 0; od < OD; ++od) {
		for (size_t p = 0; p < ODpaths[od]; ++p) {
			test = abs(xNew[od][p] - xOld[od][p]) / xOld[od][p];
			if (test > max) {
				max = test;
			}
		}
	}
	return max;
}