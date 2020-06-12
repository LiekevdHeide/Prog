#include "printingResults.h"

#include <vector>
#include <fstream>

using namespace std;

void printSchedule(ofstream& write, size_t T, size_t M, vector<size_t>& startTimes) {//print schedule: maintenance, time

	for (size_t m = 0; m < M; ++m) {
		write << "M" << m << ' ';
	}
	write << '\n';
	for (size_t m = 0; m < M; ++m) {
		write << startTimes[m] << ' ';
	}
	write << '\n';
	return;
}