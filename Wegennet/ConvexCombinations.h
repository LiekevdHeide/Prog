#pragma once
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"

void convexCombinations(ScheduleAndFlows& Flow, RoadNetwork& Roads, double convergenceCriterion, double accuracyStepsize);

size_t findShortestPath(RoadNetwork, size_t od, std::vector<std::vector<double>> arcFlows);

double calculateConvergenceTest(std::vector<std::vector<double>> &xOld, std::vector<std::vector<double>> &xNew, RoadNetwork &Roads);

//double findOptimalStepsize(std::vector<std::vector<double>> &direction, std::vector<std::vector<double>>& oldFlow, RoadNetwork &Road);

double findGoodStepsize(std::vector<std::vector<double>>& direction, std::vector<std::vector<double>>& oldFlow, RoadNetwork& Road, double epsilon);