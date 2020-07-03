#pragma once
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"


double timeMoveNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod, double bigC);
void shakeTimeNBH(MaintenanceActivities& Maintenance, ScheduleAndFlows& Sched, size_t runoutT);

double swapNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod, double bigC);
void shakeSwapNBH(MaintenanceActivities Maintenance, ScheduleAndFlows Sched);

void VNS(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, ScheduleAndFlows& equilibrium, vector<vector<vector<double>>>& touristAlternativeFlowsPerwholeState, string resultPrint, size_t runOutPeriod, size_t numSmallStep, double bigCost, double maxTime, size_t maxIterations);