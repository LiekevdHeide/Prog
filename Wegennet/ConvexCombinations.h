#pragma once
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"

void convexCombinations(ScheduleAndFlows& Flow, RoadNetwork& Roads, double convergenceCriterion);

size_t findShortestPath(RoadNetwork, size_t od, std::vector<double> pathFlows);

double calculateConvergenceTest(vector<vector<double>> xOld, vector<vector<double>> xNew, size_t OD, vector<size_t> numODpaths);
