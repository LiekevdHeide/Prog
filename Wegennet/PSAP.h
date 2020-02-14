#pragma once
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"

void adjustingTrafficFlows(size_t T, RoadNetwork& Network, ScheduleAndFlows& Schedule); //assumes at 0 no maintenance!

void findTouristFlows(vector<vector<double>>& scheduledCapacities, RoadNetwork& Roads, vector<vector<double>> &touristFlows);

void updateExpectedPathTimes(RoadNetwork& Network, ScheduleAndFlows& Schedule, size_t t, size_t od, vector<vector<double>>& pathTimes);
void proportionalSwitch(ScheduleAndFlows& Schedule, size_t t, size_t od, const vector<vector<double>>& pathTimes);

double determineFlowClosingPaths(ScheduleAndFlows& Schedule, size_t t, size_t od);
size_t findAlternativePathRecurringDrivers(ScheduleAndFlows Schedule, size_t t, size_t od, vector<vector<double>> pathTimes);
