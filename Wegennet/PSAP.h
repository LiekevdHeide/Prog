#pragma once
#include "ScheduleAndFlows.h"
#include "RoadNetwork.h"

void proportionalSwitch(size_t T, RoadNetwork& Network, ScheduleAndFlows& Schedule); //assumes at 0 no maintenance!

void findTouristFlows(vector<vector<double>>& scheduledCapacities, RoadNetwork& Roads, vector<vector<double>> &touristFlows);