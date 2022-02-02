#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <chrono>
#include "Algorithms.h"
#include "Graph.h"
using namespace std;

void generate_initial_population(vector<vector<int>> &population, int chromosome_size, int number) {
	vector<int> chromosome;
	for (int i = 0; i < chromosome_size; i++) {
		chromosome.push_back(i);
	}
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine rng(seed); // if only default_random_engine {}, then always the same combination
	for (int i = 0; i < number; i++) {
		shuffle(begin(chromosome) + 1, end(chromosome), rng);
		population.push_back(chromosome);
	}
}

int cost(Graph& graph, vector<int> &chromosome) {
	int value = 0;
	for (int i = 0; i < chromosome.size() - 1; i++) {
		value += graph.get_edge(chromosome[i], chromosome[i + 1]);
	}

	return value += graph.get_edge(chromosome[chromosome.size() - 1], 0);
}

pair<vector<int>, int> best_in_population(Graph& graph, vector<vector<int>>& population) {
	pair<vector<int>, int> ans;
	ans.second = INT_MAX;
	for (int i = 0; i < population.size(); i++) {
		if (ans.second > cost(graph, population[i])) {
			ans.first = population[i];
			ans.second = cost(graph, population[i]);
		}
	}
	return ans;
}

float fitness(int cost) {
	return 1000000 /float( cost);
}

pair<vector<int>, int> Algorithms::TSP_genetic_algorithm(Graph graph, int duration, bool tourtament_selection, int population_size, int mutation_propability, int crossover_propability) {
	auto start = std::chrono::high_resolution_clock::now();

	int CROSSOVER_PROPABILITY = crossover_propability; 
	int MUTATION_PROPABILITY = mutation_propability; 
	int ELITISM_PROPABILITY = 100;
	int POPULATION_SIZE = population_size; 

	vector<vector<int>> population;
	generate_initial_population(population, graph.get_nodes_number(), POPULATION_SIZE);

	vector<float> population_fitness;
	for (int i = 0; i < POPULATION_SIZE; i++)
		population_fitness.push_back(0);
	
	vector<vector<int>> offspring;
	for (int i = 0; i < POPULATION_SIZE; i++)
		offspring.push_back(vector<int> {0});

	pair<vector<int>, int> ans = {{},{INT_MAX}};
	while (duration > 0) {

		pair<vector<int>, int> best_in_population = {{}, {INT_MAX}}; 
		
		float fitness_sum = 0;
		if (!tourtament_selection) {
			//ROULETE WHEEL CREATION
			for (int i = 0; i < population.size(); i++) {
				int chromosome_cost = cost(graph, population[i]);
				float chromosome_fitness = fitness(chromosome_cost);
				population_fitness[i] = chromosome_fitness;
				fitness_sum += chromosome_fitness;

				if (chromosome_cost < best_in_population.second) {
					best_in_population.second = chromosome_cost;
					best_in_population.first = population[i];
				}
			}
		}
		
		if (tourtament_selection) {
			//TOURTAMENT CREATION
			for (int i = 0; i < population.size(); i++) {
				int chromosome_cost = cost(graph, population[i]);
				population_fitness[i] = chromosome_cost;

				if (chromosome_cost < best_in_population.second) {
					best_in_population.second = chromosome_cost;
					best_in_population.first = population[i];
				}
			}
		}

		for (int off = 0; off < population.size(); off += 2) {
			int parent1_pointer = 0;
			int parent2_pointer = 0;
			if (!tourtament_selection) {
				//ROULETE WHEEL PARENTS SELECTION
				float partial_sum1 = 0;
				float partial_sum2 = 0;
				float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / fitness_sum));
				float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / fitness_sum));
				
				while (!(partial_sum1 > r1 && partial_sum2 > r2)) {
					partial_sum1 += population_fitness[parent1_pointer];
					partial_sum2 += population_fitness[parent2_pointer];

					if (partial_sum1 < r1 && parent1_pointer < population_fitness.size() - 1)
						parent1_pointer++;
					if (partial_sum2 < r2 && parent2_pointer < population_fitness.size() - 1)
						parent2_pointer++;
				}
			}

			if (tourtament_selection) {
				//TOURTAMENT PARENT SELECTION K-WAY, K = 3
				vector<int> parents_pointers;
				for (int i = 0; i < 2; i++) {
					int best_parent_pointer = rand() % (population_fitness.size() - 1);
					for (int participants = 0; participants < POPULATION_SIZE / 16; participants++) {
						int random_parent_pointer = rand() % (population_fitness.size() - 1);
						if (population_fitness[random_parent_pointer] < population_fitness[best_parent_pointer]) {
							best_parent_pointer = random_parent_pointer;
						}
					}
					parents_pointers.push_back(best_parent_pointer);
				}
				parent1_pointer = parents_pointers[0];
				parent2_pointer = parents_pointers[1];
			}
			
			//PMX CROSSOVER
			vector<int> offspring1 = population[parent1_pointer];
			vector<int> offspring2 = population[parent2_pointer];

			if (rand() % 100 < CROSSOVER_PROPABILITY) { 
				int crossover_point  = (rand() % (graph.get_nodes_number())); //chromosome size
				for (int i = 0; i < crossover_point; i++) {

					int element_to_swap_off1;
					for (int j = 0; j < offspring1.size(); j++) {
						if (offspring1[j] == population[parent2_pointer][i]) {
							element_to_swap_off1 = j;
							break;
						}
					}

					int element_to_swap_off2;
					for (int k = 0; k < offspring2.size(); k++) {
						if (offspring2[k] == population[parent1_pointer][i]) {
							element_to_swap_off2 = k;
							break;
						}
					}
					
					iter_swap(offspring1.begin() + i, offspring1.begin() + element_to_swap_off1);
					iter_swap(offspring2.begin() + i, offspring2.begin() + element_to_swap_off2);
				}
			}
			
			//SWAP MUTATION 
			if (rand() % 100 < MUTATION_PROPABILITY) {
				iter_swap(offspring1.begin() + max(1, rand() % graph.get_nodes_number()), offspring1.begin() + max(1, rand() % graph.get_nodes_number()));
				iter_swap(offspring2.begin() + max(1, rand() % graph.get_nodes_number()), offspring2.begin() + max(1, rand() % graph.get_nodes_number()));
			}

			if (POPULATION_SIZE % 2 == 1 && off == graph.get_nodes_number() - 1) {
				if (rand() % 2 == 1) {
					offspring[off] = offspring1;
				}
				else {
					offspring[off] = offspring2;
				}
			}
			else {
				offspring[off] = offspring1;
				offspring[off + 1] = offspring2;
			}
		}
		
		//ELITISM
		if (rand() % 100 < ELITISM_PROPABILITY) { // curenttly not used
			offspring[rand() % offspring.size()] = best_in_population.first;
		}

		//TOTAL MUTANT
		if (rand() % 100 < 0) {
			unsigned seed = chrono::system_clock::now().time_since_epoch().count();
			default_random_engine rng(seed); 

			shuffle(begin(best_in_population.first) + 1, end(best_in_population.first), rng);
			offspring[rand() % offspring.size()] = best_in_population.first;
		}

		if (best_in_population.second < ans.second) {
			ans = best_in_population;
		}
		population = offspring;
	
		auto stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = stop - start;
		if (elapsed.count() > duration) {
			break;
		}
	}

	return ans;
}

////////////////////////////////////////////////////////// TABU SEARCH //////////////////////////////////////////////////////////////
struct Solution
{
public:
	Solution() {}

	Solution(vector<int> ANS_INIT, int cost_init) {
		ANS = ANS_INIT;
		cost = cost_init;
	}

	vector<int> ANS;
	int cost;
};

class TabuSolution
{
public:
	Solution solution;
	int tabu_tenure;
	unsigned long long hash;

	TabuSolution(int new_tabu_tenure, Solution new_solution, unsigned long long new_hash) {
		this->tabu_tenure = new_tabu_tenure;
		this->solution = new_solution;
		this->hash = new_hash;
	}

	bool is_duplicate(Solution solution_compare) {

		for (int i = 0; i < solution.ANS.size(); i++) {
			if (solution.ANS[i] != solution_compare.ANS[i])
				return false;
		}
		return true;
	}

	bool is_duplicate_hash(Solution solution_compare, unsigned long long hash_compare) {

		if (hash != hash_compare) {
			return false;
		}
		return is_duplicate(solution_compare);
	}
};

unsigned long long hash_vector(vector<int> ANS) { // better hash function here, this is slow couse of pow()
	unsigned long long hash = 0;
	int power = 0;
	for (int i = 0; i < ANS.size(); i++) {
		hash += ANS[i] * pow(10, power);
		power++;
		if (power > 18)
			power = 0;
	}
	return hash;
}

int cost_function(vector<int> ANS, Graph& graph) {
	int value = 0;
	for (int i = 0; i < ANS.size() - 1; i++) {
		value += graph.get_edge(ANS[i], ANS[i + 1]);
	}

	return value += graph.get_edge(ANS[ANS.size() - 1], 0);
}

Solution greedy_solution(Graph& graph) {
	vector<int> ANS;
	for (int i = 0; i < graph.get_nodes_number(); i++) {
		ANS.push_back(i);
	}
	Solution greedy_solution(ANS, cost_function(ANS, graph));
	return greedy_solution;
}

pair<vector<int>, int> Algorithms::tabu_search(Graph graph, int duration, bool diversification, int tabu_tenure) {
	auto start = std::chrono::high_resolution_clock::now();

	int const TABU_TENURE = tabu_tenure; // sqrt(graph.get_nodes_number()) // graph.get_nodes_number() // pow(graph.get_nodes_number(), 2)
	vector<TabuSolution> TABU_LIST;

	Solution best_solution;
	vector<Solution> LOCAL_MIN; // keeps best solutions from explored local minimums

	best_solution = greedy_solution(graph);
	
	Solution current_solution = best_solution; // its fine couse Solution do not have "new... - memory from heap" and vector will copy proper automaticly
	Solution best_neighbour;
	while (duration > 0) {

		best_neighbour.cost = INT_MAX;
		for (int i = 1; i < graph.get_nodes_number() - 1; i++) {
			for (int j = i + 1; j < graph.get_nodes_number(); j++) {
				Solution neighbour = current_solution;
				iter_swap(neighbour.ANS.begin() + i, neighbour.ANS.begin() + j);
				neighbour.cost = cost_function(neighbour.ANS, graph);

				if (neighbour.cost < best_neighbour.cost) {
					bool is_on_tabu_list = false;
					for (int j = 0; j < TABU_LIST.size(); j++) {
						if (TABU_LIST[j].is_duplicate(neighbour)) { // if (TABU_LIST[j].is_duplicate_hash(neighbour, hash_vector(neighbour.ANS))) // if (TABU_LIST[j].is_duplicate(neighbour))
							is_on_tabu_list = true;
							break;
						}
					}

					if (!is_on_tabu_list) {
						best_neighbour = neighbour;
					}
				}
			}
		}

		if (best_neighbour.cost < best_solution.cost) {
			best_solution = best_neighbour;
		}
		else if (best_neighbour.cost == best_solution.cost) {
			if (diversification) {
				auto rng = default_random_engine{};
				std::shuffle(begin(best_neighbour.ANS) + 1, end(best_neighbour.ANS), rng);
				best_neighbour.cost = cost_function(best_neighbour.ANS, graph);

				LOCAL_MIN.push_back(best_solution);
				best_solution.cost = INT_MAX;
			}
			else {
				cout << endl << "STUCK IN LOCAL MIN, SWITCH ON DIVERSIFICATION OR INCRESE TABU TENURE" << endl;
				LOCAL_MIN.push_back(best_solution);
				break;
			}
		}
		current_solution = best_neighbour;

		for (int i = 0; i < TABU_LIST.size(); i++) {
			TABU_LIST[i].tabu_tenure = TABU_LIST[i].tabu_tenure - 1;
			if (TABU_LIST[i].tabu_tenure <= 0) {
				TABU_LIST.erase(TABU_LIST.begin() + i);
				i--;
			}
		}

		TabuSolution tabu_solution(TABU_TENURE, current_solution, 0); // hash_vector(current_solution.ANS)
		TABU_LIST.push_back(tabu_solution);

		auto stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = stop - start;
		if (elapsed.count() > duration) {
			break;
		}
	}

	sort(LOCAL_MIN.begin(), LOCAL_MIN.end(), [](const Solution& lhs, const Solution& rhs) {
		return lhs.cost < rhs.cost;
		});
	//cout << endl << "VISITED LOCAL MINIMUMS (" << LOCAL_MIN.size() << "):" << endl;
	//for (int i = 0; i < LOCAL_MIN.size(); i++)
	//	cout << LOCAL_MIN[i].cost << " ";
	//cout << endl << endl;

	int increse_tenure_criterium = 0;
	for (int i = 1; i < LOCAL_MIN.size(); i++) {
		if (LOCAL_MIN[i].cost == LOCAL_MIN[i - 1].cost)
			increse_tenure_criterium++;
	}
	if (increse_tenure_criterium > 1000)
		cout << "ALGORITHM RECOMENDS YOU TO INCRESE TABU TENURE FOR BETTER RESULTS" << endl << endl;

	best_solution = LOCAL_MIN[0];
	return make_pair(best_solution.ANS, best_solution.cost);
}
