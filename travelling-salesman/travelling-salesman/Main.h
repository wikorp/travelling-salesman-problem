#include <iostream>
#include "Tester.h"
#include "Graph.h"
#include "Algorithms.h"
using namespace std;

int main() {
	bool menu_on = true;
	while (menu_on) {
		int option = 0;
		Tester tester;
		while (option != 1 && option != 2 && option != 3) {
			cout << "MAIN MENU" << endl;
			cout << "1.Test solution" << endl;
			cout << "2.Time mesurments" << endl;
			cout << "3.Exit" << endl;
			cin >> option;
			cout << endl;
		}

		switch (option) {
		case 1:
			tester.test();
			break;
		case 2:
			//tester.time_test();
			break;
		case 3:
			menu_on = false;
			break;
		}
	}
	return 0;
}
