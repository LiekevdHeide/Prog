#pragma once
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"

void convexCombinations(ScheduleAndFlows& Flow, RoadNetwork& Roads, double convergenceCriterion, double accuracyStepsize, size_t time);

size_t findShortestPath(RoadNetwork, size_t od, std::vector<std::vector<double>> arcFlows, std::vector<std::vector<double>> actualCapacities, size_t numAvailablePaths, vector<size_t> availablePaths);

double calculateConvergenceTest(std::vector<std::vector<double>> &xOld, std::vector<std::vector<double>> &xNew, RoadNetwork &Roads, std::vector<std::vector<double>> actualCapacities, std::vector<size_t> numAvailablePaths, std::vector<std::vector<size_t>> availablePaths);

//double findOptimalStepsize(std::vector<std::vector<double>> &direction, std::vector<std::vector<double>>& oldFlow, RoadNetwork &Road);

double findGoodStepsize(std::vector<std::vector<double>>& direction, std::vector<std::vector<double>>& oldFlow, RoadNetwork& Road, std::vector<std::vector<double>> actualCapacities, double epsilon);