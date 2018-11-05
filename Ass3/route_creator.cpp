#include "route_creator.h"
#include <iostream>
#include <string>


using namespace std;

void RouteCreator::Create(const Reader& r) {
	int status = 0;
	while (status == 0) {
		string ship;
		cout << "Enter ship name" << endl;
		cin >> ship;
		// check if ship exists
		if (!r.contains_ship(ship)) {
			status = 0;
			break;
		}
	}
}
