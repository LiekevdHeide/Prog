#pragma once
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"

void adjustingTrafficFlows(size_t T, RoadNetwork& Network, ScheduleAndFlows& Schedule, size_t numSmallSteps); //assumes at 0 no maintenance!

//void updateExpectedPathTimes(RoadNetwork& Network, ScheduleAndFlows& Schedule, size_t t, size_t od, vector<vector<double>>& pathTimes);
//void proportionalSwitch(ScheduleAndFlows& Schedule, size_t t, size_t od, const vector<vector<double>>& pathTimes);

void updateExpectedPathTimes(RoadNetwork& Network, size_t od, size_t numAvailableRoutes, vector<size_t>& availableRoutes, vector<vector<double>>& scheduledCapacities, vector<vector<double>>& arcFlows, vector<double>& pathTimes);
void proportionalSwitch(size_t numAvailableRoutes, const vector<size_t>& availableRoutes, const vector<double>& pathFlowOld, const vector<double>& pathTimes, vector<double>& pathFlowNew);

double determineFlowClosingPaths(ScheduleAndFlows& Schedule, size_t t, size_t od);
size_t findAlternativePathRecurringDrivers(ScheduleAndFlows &Schedule, size_t t, size_t od, vector<vector<double>> &pathTimes);

//void findAlternativeArcFlowsTourists(RoadNetwork& Network, const vector<vector<double>>& currentCapacities, vector<vector<double>>& arcFlows); Unused: now use the CPLEX variant in TouristAlternative.h
