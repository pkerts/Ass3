#include "route_creator.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;

void RouteCreator::Create(const Reader& r, const Travel_Times& t) {
	ofstream route("route.txt");
	int status = 111;
	auto time = 0;
	while (status != 0) {
		int ship;
		cout << "Ships:" << endl;
		r.dump_ships();
		cout << "Select ship" << endl;
		cin >> ship;
		switch (ship) {
			case 1: route << "Andromeda Ascendant\t"; break;
			case 2: route << "USSC Discovery One\t"; break;
			case 3: route << "Galactica\t"; break;
			case 4: route << "Heart of Gold\t"; break;
			case 5: route << "Millenium Falcon\t"; break;
			case 6: route << "Moya\t"; break;
			case 7: route << "NSEA Protector\t"; break;
			case 8: route << "Red Dwarf\t"; break;
			case 9: route << "Serenity\t"; break;
			case 10: route << "USS Enterprise\t"; break;
		default: ;
		}
		cout << "\nSelect origin" << endl;
		int origin = 0;
		cout << "Planets:" << endl;
		t.dump(0);
		cin >> origin;
		cout << endl;
		auto origin_name = t.dump(origin);
		route << origin_name << '\t' << time << '\t';
		cout << "Select destination" << endl;
		int destination = 0;
		cin >> destination;
		auto travel_pair = t.travel_time(origin, destination);
		route << travel_pair.first << '\t' << (time + travel_pair.second) << endl;
		cout << "type 0 to halt, type 1 for another ship" << endl;
		cin >> status;
	}
}
