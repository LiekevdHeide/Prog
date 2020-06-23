
#include "costCalculationFunctions.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void shiftToOne(size_t M, vector<size_t> &startTimes) {//check if maintenance starts at t=1, if not shift all back
	
	size_t minTime = *min_element(startTimes.begin(), startTimes.end());


	if (minTime != 1) {
		for (size_t m = 0; m < M; ++m) {
			startTimes[m] -= (minTime - 1);
		}
	}

	return;
}