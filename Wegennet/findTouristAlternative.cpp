#include "TouristAlternative.h"


#include <ilcplex/ilocplex.h>
#include <vector>
#include <string>

using namespace std;

vector<size_t> findTouristAlternative(size_t N, vector<vector<double>> &travelTimes, vector<vector<double>> &actualCapacities, vector<size_t> interruptedRoute){

	for (size_t p = 0; p < interruptedRoute.size(); ++p) {
		cout << interruptedRoute[p];
	}

	IloEnv env;
	IloModel model(env);
	

	//create variables:
	IloArray<IloNumVarArray> arcs(env, N);
	for (size_t i = 0; i < N; ++i) {
		arcs[i] = IloNumVarArray(env, N, 0, 1, IloNumVar::Int);

		string name = "arc";
		name += to_string(i);
		arcs[i].setNames(name.c_str());
		model.add(arcs[i]);
	}

	
	//objective function:
	IloExpr objFunction(env);
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j)
			objFunction += travelTimes[i][j] * arcs[i][j];
	}

	for (size_t r = 0; r < interruptedRoute.size() - 1; ++r) {
		objFunction -= arcs[interruptedRoute[r]][interruptedRoute[r + 1]]; //ADD FACTOR!!
	}

	model.add(IloMinimize(env, objFunction));
	objFunction.end();


	//add constraints:
	//in=out
	for (size_t i = 0; i < N; ++i) {
		if (i != interruptedRoute[0] && i != interruptedRoute.back()) {
			IloExpr allIn(env);
			IloExpr allOut(env);
			for (size_t j = 0; j < N; ++j) {
				allIn += arcs[i][j];
				allOut += arcs[j][i];
			}
			model.add(allIn == allOut);
			//model.add(allIn <= 1);
			//model.add(allOut <= 1);
			allIn.end();
			allOut.end();
		}
	}

	//leave o = 1, arrive d = 1
	IloExpr leaveOrigin(env);
	IloExpr arriveDestination(env);
	for (size_t j = 0; j < N; ++j) {
		leaveOrigin += arcs[interruptedRoute[0]][j];
		arriveDestination += arcs[j][interruptedRoute.back()];
	}
	model.add(leaveOrigin == 1);
	model.add(arriveDestination == 1);
	leaveOrigin.end();
	arriveDestination.end();

	//arrive o = 0, leave d = 0
	IloExpr leaveDestination(env);
	IloExpr arriveOrigin(env);
	for (size_t j = 0; j < N; ++j) {
		leaveDestination += arcs[interruptedRoute.back()][j];
		arriveOrigin += arcs[j][interruptedRoute[0]];
	}
	model.add(leaveDestination == 0);
	model.add(arriveOrigin == 0);
	leaveDestination.end();
	arriveOrigin.end();


	//only use available roads
	for(size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j) {
			model.add(arcs[i][j] <= actualCapacities[i][j]);
		}


	// make + solve the cplex
	IloCplex cplex(model);
	cplex.exportModel("ObjectiveAndConstraints.lp");
	cplex.solve();

	//get results from IP
	IloNum objVal = cplex.getObjValue();
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			cout << cplex.getValue(arcs[i][j]) << ' ';
		}
		cout << '\n';
	}

	size_t currentVertice = interruptedRoute[0];
	vector<size_t> newRoute;
	newRoute.push_back(interruptedRoute[0]);
	while (currentVertice != interruptedRoute.back()) {//while current != final entry in interruptedRoute
		for (size_t v = 0; v < N; ++v) {
			if (cplex.getValue(arcs[currentVertice][v]) > 0) {
				newRoute.push_back(v);
				currentVertice = v;
				break;
			}
		}
	}


	model.end();
	env.end();
	return newRoute;
}