#pragma once
#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"

#include <vector>

double totalTravelTime(RoadNetwork &Network, size_t T, std::vector<std::vector<std::vector<double>>>& scheduledCapacities, std::vector<std::vector<std::vector<double>>>& arcFlows);

double costFromSchedule(RoadNetwork& Network, MaintenanceActivities& Maintenance, ScheduleAndFlows& Schedule, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, size_t numSmallStep, double biGC);

//Replaces everything in Schedule, using startTimes + returns double costs
double costFromStarttimes(RoadNetwork& Network, MaintenanceActivities& Maintenance, ScheduleAndFlows& Schedule, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, size_t numSmallStep, double biGC);
