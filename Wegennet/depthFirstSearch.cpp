#include "RoadNetwork.h"
#include "DepthFirstSearchFunctions.h"

#include <vector>
#include <cstddef> //necessery for size_t!!

using namespace std;

void depthFirstSearch(RoadNetwork &Network) {
	cout << "Found routes ";
	for (size_t od = 0; od < Network.numberODpairs; ++od) {
		cout << od << ":\n";
		findAllPaths(od, Network);

	}



	return;
}