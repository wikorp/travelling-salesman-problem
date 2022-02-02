#include <iostream>
#include "tester.h"
using namespace std;

int main() {
	bool menu_on = true;
	while (menu_on) {
		int option = 0;
		Tester tester;
		while (option != 1 && option != 2 && option != 3) {
			cout << "MAIN MENU" << endl;
			cout << "1.Test solution" << endl;
			cout << "2.Exit" << endl;
			cin >> option;
			cout << endl;
		}

		switch (option) {
		case 1:
			tester.test();
			break;
		case 2:
			menu_on = false;
			break;
		}
	}
	return 0;
}

