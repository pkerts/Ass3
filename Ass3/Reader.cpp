#include "Reader.h"
#include <fstream>
#include <map>
#include <string>

using namespace std;

Reader::Reader(std::istream& in, Travel_Times* constraints) : in(in), constraints(constraints) {
	galaxy = load();
}

Galaxy* Reader::load() {
	auto galaxy = new Galaxy();

	// BEGIN AND END ITERATORS FOR ACCESS TO CONSTRAINTS THE PLANETS MAP
	const auto begin = constraints->begin();
	const auto end = constraints->end();

	// FOR LOOP FOR PLANETS MAP TO ADD PLANETS TO GALAXY and also to PLANETS MAP (of planet names and Planet objects.)
	for (auto i = begin; i != end; ++i) {
		// for loop (looping through planets map in Travel_Times constraints , conduits.txt....
		auto planet = new Planet(*i);
		galaxy->add(planet);
		planets.emplace(*i, planet);
		// having our newly created planet object and also access to our planet name strings from the map in Travel_Times
		// since we are in the map, we Now create the pairs that go in the Reader's planets map. (planet name & planet object*).
	}

	ifstream ship_names("ship-names.txt");
	string ship;
	while (getline(ship_names, ship)) {
		ships.emplace(ship, galaxy->fleet.add(ship));
		// we both emplace the ship name/ship ID to the Reader's ship name/ship id map named ships, 
		// we also add the ship strings to the fleet. (which conveniently returns the id)
	}
	return galaxy;
}

// FUNCTION TO DUMP ALL SHIP NAMES PRECEDED BY INDEX
void Reader::dump_ships() const {
	auto count = 1;
	for (auto i : ships) {
		cout << count << " " << i.first << endl;
		count++;
	}
}

void Reader::dump_current_info() const {
	if (previous_arrival_time == 0) {
		cerr << "previous_ship_id: NULL" << endl;
		cerr << "previous_destination_planet: NULL" << endl;
		cerr << "previous_arrival_time: NULL" << endl << endl;
	} else {
		cerr << "previous_ship_id: " << previous_ship_id << endl;
		cerr << "previous_destination_planet: ";
		if (previous_destination_planet) cerr << previous_destination_planet->name << endl;
		else cerr << "0" << endl;
		cerr << "previous_arrival_time: " << previous_arrival_time << endl << endl;
	}
	cerr << "ship_id: " << ship_id << endl;
	cerr << "departure_planet: " << departure_planet->name << endl;
	cerr << "departure_time: " << departure_time << endl;
	cerr << "destination_planet: " << destination_planet->name << endl;
	cerr << "arrival_time: " << arrival_time << endl << endl;
}

bool Reader::ReadAndVerify() {
	while (get_record()) {
		if (!validate()) return false;
		Leg l(ship_id, departure_time, arrival_time);
		if (!(edges.count(departure_planet) && edges[departure_planet].count(destination_planet))) {
			edges[departure_planet][destination_planet] = new Edge(destination_planet);
		}
		edges[departure_planet][destination_planet]->add(l); // here we finalize the edges
	}
	// once edges are finalized and legs are added to them. we can add them to the planets using the public add() method
	for (auto i : galaxy->planets) {
		for (auto k : galaxy->planets) {
			if (i != k) {
				if (edges.find(i) != edges.end()) {
					auto edge = edges[i].find(k);
					if (edge != edges[i].end()) { i->add(edge->second); }
				}
			}
		}
	}
	return true;
}

void Reader::Dijkstras() const {
	galaxy->search();
}

bool Reader::get_record() {
	if (getline(in, current_input_line)) {
		if (arrival_time == 0) {
			previous_ship_id = -1; // if this is our first record, set previous ship id to -1
			// this is so our validate runs properly. it knows if its a new route by comparing current ship id to previous.
			// currently, everything initializes to 0 (which is fine) but the start of the route of the first ship may be
			// id = 0
		}
		else previous_ship_id = ship_id;
		// update previous ship id to the previous current ship
		previous_destination_planet = destination_planet;
		previous_arrival_time = arrival_time;

		string delimiter = "\t";
		size_t pos;
		// We break the string down for the 5 pieces of information we need using a token-based system
		auto i = 1; // Counter which goes up until 5
		while ((pos = current_input_line.find(delimiter)) != string::npos) {
			// loop for the file
			auto token = current_input_line.substr(0, pos);
			switch (i) {
			case 1: ship_id = ships.find(token)->second; break;
			case 2: departure_planet = planets.find(token)->second; break;
			case 3: departure_time = stoi(token); break;
			case 4: destination_planet = planets.find(token)->second; break;
			default: ; }
			current_input_line.erase(0, pos + delimiter.length()); // advancing through the line being processed
			i++;
		}
		// At the end of this process, our current line/string is axed so that all 4 former pieces of information are already saved in their respective 
		// correct locations and removed from the string/line being processed, our 5th piece of info is all that's left. We save it.
		arrival_time = stoi(current_input_line);

		return true;
	}
	return false; // Returns false once no more input to process
}

bool Reader::validate() const {
	if (!(departure_time >= previous_arrival_time + TURNAROUND_TIME)) {
		// checks to make sure there's a correct layover!
		if (ship_id == previous_ship_id) {
			// if no layover (4 hours) detected. it must be a new ship then.. check
			cerr <<
				"\nERROR: Current leg is not a valid continuation of the previous leg / is not the beginning of the route for another ship.\ninfo: "
				<< endl;
			dump_current_info();
			return false;
			// we only got here if neither the above two are true. this false triggers our eventual EXIT_FAILURE in main..
		}
	}
	// CHECK MAKE SURE ROUTE.TXT TRAVEL TIME MATCHES CONDUITS.TXT TRAVEL TIME
	if (arrival_time - departure_time != constraints->transit_time(departure_planet->name, destination_planet->name)) {
		cerr <<
			"\nERROR: Travel time does not seem right. Please step to the side for a random space search\ndebug info: "
			<< endl;
		dump_current_info();
		return false;
	}
	cerr << "ALL GOOD:\n" << endl; // just a simple message all good after each entry 
	dump_current_info();
	return true;
}
