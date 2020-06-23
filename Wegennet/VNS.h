#pragma once
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"


void timeMoveNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod, double bigC);
void shakeTimeNBH(MaintenanceActivities& Maintenance, ScheduleAndFlows& Sched, size_t runoutT);

void swapNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, std::vector<std::vector<std::vector<double>>>& touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod, double bigC);
void shakeSwapNBH(MaintenanceActivities Maintenance, ScheduleAndFlows Sched);