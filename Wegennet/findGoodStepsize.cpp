#include "RoadNetwork.h"

#include <vector>
#include <iostream>
#include <cstddef> //necessery for size_t!!

using namespace std;
double findGoodStepsize(std::vector<std::vector<double>>& direction, std::vector<std::vector<double>>& oldFlow, RoadNetwork& Road, vector<vector<double>> currentCapacities, double epsilon) {
	
	//for diff values of alpha:
	double lowerBound = 0.0;
	double upperBound = 1.0;
	double midPoint = 0.5;
	double objectiveValue = 1.0;

	while (upperBound - lowerBound > epsilon) {
		objectiveValue = 0.0;
		midPoint = (lowerBound + upperBound) / 2;

		for (size_t a = 0; a < Road.vertices; ++a) {
			for (size_t b = 0; b < Road.vertices; ++b) {
				if (currentCapacities[a][b] > 0) {
					objectiveValue += (direction[a][b] - oldFlow[a][b]) * Road.travelTimeRoad(currentCapacities[a][b], Road.freeFlowTimes[a][b], oldFlow[a][b] + midPoint * (direction[a][b] - oldFlow[a][b]));
				}
			}
		}
		//cout << objectiveValue << ' ';
		//reduce the interal: the function is monotone
		if (objectiveValue > 0) {
			upperBound = midPoint;
		}
		else {
			lowerBound = midPoint;
		}
		//cout << upperBound << "--" << lowerBound << "   ";
	}
	//cout << "stepsize: " << ((upperBound + lowerBound) / 2) << '\n';
	return ((upperBound + lowerBound) / 2); // accuracy is: upper - lower / 2
}