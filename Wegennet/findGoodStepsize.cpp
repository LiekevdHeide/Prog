#include "RoadNetwork.h"

#include <vector>
#include <iostream>

using namespace std;
double findGoodStepsize(std::vector<std::vector<double>>& direction, std::vector<std::vector<double>>& oldFlow, RoadNetwork& Road, double epsilon) {
	double midPoint = 0.5;
	//double currentAlpha = 0.4;

	//for diff values of alpha:
	double lowerBound = 0.0;
	double upperBound = 1.0;
	double objectiveValue = 1.0;

	while (upperBound - lowerBound > epsilon) {
		objectiveValue = 0.0;
		midPoint = (lowerBound + upperBound) / 2;

		for (size_t a = 0; a < Road.vertices; ++a) {
			for (size_t b = 0; b < Road.vertices; ++b) {
				objectiveValue += (direction[a][b] - oldFlow[a][b]) * Road.freeFlowTimes[a][b] * (1 + 0.15 * pow(oldFlow[a][b] + midPoint * (direction[a][b] - oldFlow[a][b]) / Road.standardCapacities[a][b], 4));
			}
		}
		if (objectiveValue > 0) {
			lowerBound = midPoint;
		}
		else {
			upperBound = midPoint;
		}
	}
	cout << "stepsize: " << ((upperBound + lowerBound) / 2) << ' ';
	return ((upperBound + lowerBound) / 2); // accuracy is: upper - lower / 2
}