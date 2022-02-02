#include <vector>
#include <iostream>
using namespace std;

#pragma once
class Graph
{
public:
	void read(string path);
	void clean();
	int get_nodes_number();
	void set_nodes_number(int size);
	int get_edge(int i, int j);
	void set_edge(int i, int j, int value);

private:
	vector<vector<int>> graph;
};

