#include <vector>
#include "Graph.h"
using namespace std;

#pragma once
class Algorithms
{
public:
	static pair<vector<int>, int> TSP_genetic_algorithm(Graph graph, int duration, bool tourtament_selection, int population_size, int mutation_propability, int crossover_propability);
	static pair<vector<int>, int> tabu_search(Graph graph, int duration, bool diversification, int tabu_tenure);
};

