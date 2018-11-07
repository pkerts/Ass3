#include "Reader.h"
#include <fstream>

using namespace std;

Reader::Reader(std::istream & in, Travel_Times * constraints) : in(in), constraints(constraints)
{
	/*Fleet f;
	ifstream ships_in("ship-names.txt");
	string ship;
	while (getline(ships_in, ship)) {
		ships[ship] = f.add(ship);
	}*/
	galaxy = load();
}

Galaxy* Reader::load() {
	auto galaxy = new Galaxy();

	const auto begin = constraints->begin();
	const auto end = constraints->end();
	for (auto i = begin; i != end; ++i) {
		auto planet = new Planet(*i);
		galaxy->add(planet);
		planets.emplace(*i, planet);
	}

	ifstream ship_names("ship-names.txt");
	string ship;
	while (getline(ship_names, ship)) {
		ships.emplace(ship, galaxy->fleet.add(ship));
	}

	// At this point there should be 23ish/24ish planets in the data structure holding planets here and there should be about 10 ships holding ship ldhfaskljdh

	return galaxy;
}

bool Reader::contains_ship(const std::string& ship_name) const {
	return ships.count(ship_name);
}

void Reader::dump_ships() const {
	int count = 1;
	for (auto i : ships) {
		cout << count << " " << i.first << endl;
		count++;
	}
}

void Reader::dump_current_info() const {
	// Previous SHIP_ID
	cerr << "previous_ship_id: " << previous_ship_id << endl;

	// Previous DESTINATION PLANET *
	cerr << "previous_destination_planet: ";
	if (previous_destination_planet) cerr << previous_destination_planet->name << endl;
	else cerr << "0" << endl;

	// Previous arrival time
	cerr << "previous_arrival_time: " << previous_arrival_time << endl;






	// SHIP_ID
	cerr << "ship_id: " << ship_id << endl;

	// DEPARTURE PLANET *
	cerr << "departure_planet: " << departure_planet->name << endl;

	// DEPARTURE TIME
	cerr << "departure_time: " << departure_time << endl;

	// destination planet *
	cerr << "destination_planet: " << destination_planet->name << endl;

	// Arrival time
	cerr << "arrival_time: " << arrival_time << endl;

}

bool Reader::ReadAndVerify() {
	while (get_record()) {
		if (!validate()) return false;
	}
	return true;
}

bool Reader::get_record() {
	if (getline(in, current_input_line)) {
		previous_ship_id = ship_id;
		previous_destination_planet = destination_planet;
		previous_arrival_time = arrival_time;
		
		
		string delimiter = "\t";
		size_t pos = 0;
		string token;
		int i = 1;
		while ((pos = current_input_line.find(delimiter)) != string::npos) {
			token = current_input_line.substr(0, pos);
			switch (i) {
			case 1: ship_id = ships.find(token)->second; break;
			case 2: departure_planet = planets.find(token)->second; break;
			case 3: departure_time = stoi(token); break;
			case 4: destination_planet = planets.find(token)->second; break;
			default: ;
			}
			current_input_line.erase(0, pos + delimiter.length());
			i++;
		}
		arrival_time = stoi(current_input_line);
		return true;
	}
	return false;
}

bool Reader::validate() const {
	if (!(departure_time == (previous_arrival_time+4))) {
		if (!(departure_time == 0 && ship_id != previous_ship_id)) {
			cerr << "\nERROR: Current leg is not a valid continuation of the previous leg / is not the beginning of the route for another ship.\ninfo: " << endl;
			dump_current_info();
			return false;
		}
	}
	if (arrival_time - departure_time != constraints->transit_time(departure_planet->name, destination_planet->name)) {
		cerr << "\nERROR: Travel time does not seem right. Please step to the side for a random space search\ndebug info: " << endl;
		dump_current_info();
		return false;
	}
	cerr << "ALL GOOD:" << endl;
	dump_current_info();
	return true;
}
