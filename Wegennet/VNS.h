#pragma once
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"
#include "VNSparameters.h"

#include <cstddef> 
#include <vector>


double timeMoveNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, double PSAPalpha, size_t numSmallStep,  double bigC);
void shakeTimeNBH(MaintenanceActivities& Maintenance, ScheduleAndFlows& Sched);

double swapNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, double PSAPalpha, size_t numSmallStep, double bigC);
void shakeSwapNBH(MaintenanceActivities Maintenance, ScheduleAndFlows Sched);

void VNS(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, VNSparameters &VNSinput, ScheduleAndFlows& equilibrium, vector<vector<vector<double>>>& touristAlternativeFlowsPerwholeState, string resultPrint,  double PSAPalpha, size_t numSmallStep, double bigCost);