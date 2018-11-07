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
	while (get_record()) {
		;
	}
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
