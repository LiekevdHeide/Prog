#include "TouristAlternative.h"

#include <vector>

using namespace std;

void adjustTouristArcFLows(size_t startTime, size_t T, size_t M, const vector<vector<size_t>> &binarySchedule, const vector<vector<vector<double>>> &touristAlternativeFlowsperWholeState, vector<vector<vector<double>>> &arcFlowTourist) {

	size_t wholeM;
	for (size_t t = startTime; t < T; ++t) {
		wholeM = 0;
		for (size_t m = 0; m < M; ++m) {
			if (binarySchedule[t][m] > 0) {
				wholeM += pow(2, m);
			}
		}
		arcFlowTourist[t] = touristAlternativeFlowsperWholeState[wholeM];//already multiplied by rho
	}
	return;
}