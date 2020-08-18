#include "ScheduleCheckFunctions.h"

#include <vector>
#include <iostream>
#include <cstddef> //necessery for size_t!!

using namespace std;

void wholeScheduleToMaintenance(size_t T, size_t M, size_t scheduleValue, vector<size_t> &startTimes) {
	size_t remainder = 0;
	
	for (size_t m = 0; m < M; ++m) {//for each maintenance
		//assert(i < M && "out of bounds");
		if (scheduleValue > 0) {
			startTimes[m] = scheduleValue % T;
			scheduleValue = scheduleValue / T;
		}
		else {
			startTimes[m] = 0; 
		}
	}
	return;
}
