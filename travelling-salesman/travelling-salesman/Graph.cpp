#include <fstream>
#include <iostream>
#include "Graph.h"
using namespace std;

void Graph::read(string path) {
	int size, distance;
	clean();
	ifstream file;
	file.open(path);

	if (file.is_open())
	{
		file >> size;
		if (file.fail())
			cout << "file error" << endl;
		else {
			graph.resize(size);
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					file >> distance;
					if (file.fail())
					{
						cout << "file error - READ DATA" << endl;
						break;
					}
					else {
						graph[i].push_back(distance);
					}
				}
			}
		}
		file.close();
	}
	else {
		cout << "file error - OPEN" << endl;
	}
}

void Graph::clean() {
	graph.clear();
}

int Graph::get_nodes_number() {
	return graph.size();
}

void Graph::set_nodes_number(int size) {
	clean();
	graph.resize(size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			graph[i].push_back(0);
		}
	}
}

int Graph::get_edge(int i, int j) {
	if (i >= 0 && i < graph.size() && j >= 0 && j < graph.size())
		return graph[i][j];
	return NULL;
}

void Graph::set_edge(int i, int j, int value) {
	if (i >= 0 && i < graph.size() && j >= 0 && j < graph.size())
		graph[i][j] = value;
	else cout << "error seting edge" << endl;
}
