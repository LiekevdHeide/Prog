#pragma once

#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"


void initializeSchedule(ScheduleAndFlows&, ScheduleAndFlows&,  MaintenanceActivities&, RoadNetwork&, std::vector<std::vector<std::vector<double>>>& touristAltPerWholeState, size_t numSmallStep);