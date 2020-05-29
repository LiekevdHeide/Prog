#include "TouristAlternative.h"


#include <ilcplex/ilocplex.h>
#include <vector>
#include <string>

using namespace std;

vector<size_t> findTouristAlternative1(size_t N, double Mu, vector<vector<double>>& travelTimes, vector<vector<double>>& actualCapacities, vector<size_t> interruptedRoute) {

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
		objFunction -= Mu * travelTimes[interruptedRoute[r]][interruptedRoute[r + 1]] * arcs[interruptedRoute[r]][interruptedRoute[r + 1]]; //ADD FACTOR!!
	}

	model.add(IloMinimize(env, objFunction));
	objFunction.end();


	//add constraints:
	//in=out
	for (size_t i = 0; i < N; ++i) {
		if (i != interruptedRoute.back()) {//if not origin / destination
			IloExpr allIn(env);
			IloExpr allOut(env);
			for (size_t j = 0; j < N; ++j) {
				allOut += arcs[i][j];
				allIn += arcs[j][i];
			}
			if (i == interruptedRoute[0]) {
				model.add(allOut == allIn + 1);
			}
			else {
				model.add(allIn == allOut);
			}
			allIn.end();
			allOut.end();
		}
	}

	//leave o = 1, arrive d = 1
	IloExpr arriveDestination(env);
	for (size_t j = 0; j < N; ++j) {
		arriveDestination += arcs[j][interruptedRoute.back()];
	}
	model.add(arriveDestination == 1);
	arriveDestination.end();


	//only use available roads
	for (size_t i = 0; i < N; ++i)
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