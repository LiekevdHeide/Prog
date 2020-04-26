#pragma once
#include "RoadNetwork.h"
#include <vector>

void findTouristAlternative(size_t N, std::vector<std::vector<double>>& travelTimes, std::vector<std::vector<double>>& standardCapacities, std::vector<size_t> interrupted);

void touristAlternative(RoadNetwork Net, size_t M, std::vector<std::vector<size_t>>& locations);


