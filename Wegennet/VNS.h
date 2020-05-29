#pragma once
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"
#include "ScheduleAndFlows.h"


bool timeMoveNeighbourhood(RoadNetwork& Network, ScheduleAndFlows& Schedule, MaintenanceActivities& Maintenance, vector<vector<vector<double>>>& touristAltPerwholeState, size_t numSmallStep, size_t runoutPeriod, size_t mToShift);