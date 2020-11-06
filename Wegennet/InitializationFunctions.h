#pragma once

#include "ScheduleAndFlows.h"
#include "MaintenanceActivities.h"
#include "RoadNetwork.h"

#include <vector>
#include <cstddef> 


void initializeSchedule(ScheduleAndFlows&, ScheduleAndFlows&,  MaintenanceActivities&, RoadNetwork&, std::vector<std::vector<std::vector<double>>>& touristAltPerWholeState, double PSAPalpha, size_t numSmallStep, double bigC);