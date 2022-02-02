#include "Tester.h"
#include "Algorithms.h"

void Tester::test() {
	string filename = "null";
	int seconds = 20;
	int population_size = 0;
	int crossover_propability = 70;
	int mutation_propability = 30;
	bool diversification = true;
	int tabu_tenure = 0;
	pair<vector<int>, int> ANS;

	int option;
	while (true) {
		cout << "TEST SOLUTION MENU" << endl;
		cout << "1.Run genetic algorithm and tabu search" << endl;
		cout << "2.Set test file  (now is: " << filename << ")" << endl;
		cout << "3.Set how many seconds algorithm will works (now is: " << seconds << ")" << endl;
		cout << "4.Set population size (now is: ";
		if (population_size == 0 || population_size == graph.get_nodes_number())
			cout << "equal to graph nodes number)" << endl;
		else cout << population_size << ")" << endl;
		cout << "5.Set crossover propability (now is: " << crossover_propability << ")" << endl;
		cout << "6.Set mutation propability (now is: " << mutation_propability << ")" << endl;
		cout << "7.Set diversification (now is: ";
		if (diversification)
			cout << "on)" << endl;
		else cout << "off)" << endl;
		cout << "8.Set tabu tenure (now is: ";
		if (tabu_tenure == 0 || tabu_tenure == graph.get_nodes_number())
			cout << "equal to graph nodes number)" << endl;
		else cout << tabu_tenure << ")" << endl;
		cout << "9.Exit" << endl;
		cin >> option;

		if (option == 9) {
			cout << endl;
			break;
		}

		if (option == 1) {
			if (filename == "null") {
				cout << endl << ">set test file first<" << endl << endl;
				continue;
			}
			cout << endl;

			cout << "genethic algorithm - roulete wheel, swap mutation" << endl;
			ANS = Algorithms::TSP_genetic_algorithm(graph, seconds, false, population_size, crossover_propability, mutation_propability);
			cout << "answer after " << seconds << "seconds: " << endl;
			for (int i = 0; i < ANS.first.size(); i++) {
				cout << ANS.first[i] << "->";
			}
			cout << "0 ";
			cout << "(" << ANS.second << ")" << endl << endl;

			cout << "genethic algorithm - tourtament, swap mutation" << endl;
			ANS = Algorithms::TSP_genetic_algorithm(graph, seconds, true, population_size, crossover_propability, mutation_propability);
			cout << "answer after " << seconds << "seconds: " << endl;
			for (int i = 0; i < ANS.first.size(); i++) {
				cout << ANS.first[i] << "->";
			}
			cout << "0 ";
			cout << "(" << ANS.second << ")" << endl << endl;

			cout << "tabu search" << endl;
			ANS = Algorithms::tabu_search(graph, seconds, diversification, tabu_tenure);
			cout << "answer after " << seconds << "seconds: " << endl;
			for (int i = 0; i < ANS.first.size(); i++) {
				cout << ANS.first[i] << "->";
			}
			cout << "0 ";
			cout << "(" << ANS.second << ")" << endl << endl;
		}

		if (option == 2) {
			cout << endl << "enter file name" << endl;
			cin >> filename;
			cout << endl;
			graph.read(filename);
			population_size = graph.get_nodes_number();
		}

		if (option == 3) {
			cout << endl << "enter how many seconds algorithm will works" << endl;
			cin >> seconds;
			cout << endl;
		}

		if (option == 4) {
			cout << endl << "enter population size" << endl;
			cin >> population_size;
			cout << endl;
		}

		if (option == 5) {
			cout << endl << "enter crossover propability (%)" << endl;
			cin >> crossover_propability;
			cout << endl;
		}

		if (option == 6) {
			cout << endl << "enter mutation propability (%)" << endl;
			cin >> mutation_propability;
			cout << endl;
		}

		if (option == 7) {
			cout << endl << "enter diversification: 0-off, 1-on" << endl;
			cin >> diversification;
			cout << endl;
		}

		if (option == 8) {
			cout << endl << "enter tabu tenure" << endl;
			cin >> tabu_tenure;
			cout << endl;
		}
	}
}




