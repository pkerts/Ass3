#include "route_creator.h"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;

void RouteCreator::Create(const Reader& r, const Travel_Times& t) {
	ofstream route("route.txt");
	int status = 111;
	int time, ship, origin, destination = 0;
	string origin_name;
	string destination_name;

	while (status != 0) {
		if (status != 2) {
			time = 0;
			cout << "Ships:" << endl;
			r.dump_ships();
			cout << "Select ship" << endl;
			cin >> ship;
		}

		switch (ship) {
		case 1: route << "Ship1\t"; break;
		case 2: route << "Ship2\t"; break;
		case 3: route << "Ship3\t"; break;
		case 4: route << "Ship4\t"; break;
		case 5: route << "Ship5\t"; break;
		case 6: route << "Ship6\t"; break;
		case 7: route << "Ship7\t"; break;
		case 8: route << "Ship8\t"; break;
		case 9: route << "USS Enterprise\t"; break;
		case 10: route << "USSC Discovery One\t"; break;
		default:;
		}

		if (status !=2) {
			cout << "\nSelect origin\nPlanets:" << endl;
			t.dump(0); // Show them origin planets
			cin >> origin; // Receive origin choice
			cout << endl;
			origin_name = t.dump(origin); // Save origin name planet and also print destinations from there
			route << origin_name << '\t' << time << '\t'; // Print origin name planet and current time
			cout << "Select destination" << endl;
			cin >> destination; // From previously printed destination list from that origin planet, pick choice
			auto travel_pair = t.travel_time(origin, destination); // retrieve travel time
			destination_name = travel_pair.first; // get destination name
			time += travel_pair.second; // update time
			route << destination_name << '\t' << time << endl; // print destination name and arrival time
		}

		if (status == 2) {
			origin = destination; // Update origin digit with what we already know
			origin_name = destination_name; // Update origin name with what we already know
			time += 4; // Add 4 hour delay
			route << origin_name << '\t' << time << '\t'; // Print origin name and updated current time
			cout << "\nSelect Destination\nPlanets:" << endl;
			origin = t.obtain_origin_int(origin_name);
			t.dump(origin); // Print available destinations from new origin
			cin >> destination; // obtain new destination
			auto travel_pair = t.travel_time(origin, destination);
			destination_name = travel_pair.first; // get destination name
			time += travel_pair.second; // update time
			route << destination_name << '\t' << time << endl; // print destination name and arrival time
		}

		cout << "type 0 to halt, type 1 for another ship, type 2 to continue with same ship" << endl;
		cin >> status;
	}
}
