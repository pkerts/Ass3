#include "Reader.h"
#include <fstream>
#include <map>
#include <string>

using namespace std;

Reader::Reader(std::istream & in, Travel_Times * constraints) : in(in), constraints(constraints)
{
	galaxy = load(); // this is all that's done in the constructor. read below
	// ADDS PLANETS TO READER MAP (PLANET NAMES/PLANET OBJS) AND GALAXY VECTOR (PLANETS OBJS..) 
	// AS WELL ADDS SHIPS(to the READER ship names/ids map and also galaxy's FLEET's vector of names. (ships.) ok ,.
}

Galaxy* Reader::load() {
	auto galaxy = new Galaxy();

	// BEGIN AND END ITERATORS FOR ACCESS TO CONSTRAINTS THE PLANETS MAP
	const auto begin = constraints->begin();
	const auto end = constraints->end();

	// FOR LOOP FOR PLANETS MAP TO ADD PLANETS TO GALAXY and also to PLANETS MAP (of planet namezz and Planet objects.)
	for (auto i = begin; i != end; ++i) { // for loop (looping through planets map in Travel_Times constraints , conduits.txt....
		auto planet = new Planet(*i);
		galaxy->add(planet);
		planets.emplace(*i, planet); // having our newly created planet object and also access to our planet name strings from the map in Travel_Times
									 // since we are in the map, we Now create the pairs that go in the Reader's planets map. (planet name & planet object*).
	}

	// THIS WE HANDLE THE SHIPZZZZZ
	ifstream ship_names("ship-names.txt"); // fstream for tha ship names
	string ship; // string buffer for ze ship names
	while (getline(ship_names, ship)) { // loop through yes yes
		ships.emplace(ship, galaxy->fleet.add(ship)); // we both emplace the ship name/ship ID to the Reader's ship name/ship id map named ships, 
													  // we also add the ship strings to the fleet. (which conveniently returns the id)
	}

	// after all ZE hard work we return
	return galaxy; // RETURN
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
		// Previous SHIP_ID
		cerr << "previous_ship_id: NULL" << endl;
		// Previous DESTINATION PLANET *
		cerr << "previous_destination_planet: NULL" << endl;
		// Previous arrival time
		cerr << "previous_arrival_time: NULL" << endl << endl;
	}
	else {
		// Previous SHIP_ID
		cerr << "previous_ship_id: " << previous_ship_id << endl;
		// Previous DESTINATION PLANET *
		cerr << "previous_destination_planet: ";
		if (previous_destination_planet) cerr << previous_destination_planet->name << endl;
		else cerr << "0" << endl;
		// Previous arrival time
		cerr << "previous_arrival_time: " << previous_arrival_time << endl << endl;
	}



	// SHIP_ID
	cerr << "ship_id: " << ship_id << endl;

	// DEPARTURE PLANET *
	cerr << "departure_planet: " << departure_planet->name << endl;

	// DEPARTURE TIME
	cerr << "departure_time: " << departure_time << endl;

	// destination planet *
	cerr << "destination_planet: " << destination_planet->name << endl;

	// Arrival time
	cerr << "arrival_time: " << arrival_time << endl << endl;

}

// 
bool Reader::ReadAndVerify() {
	while (get_record()) {
		if (!validate()) return false;
		const auto leg = new Leg(ship_id, departure_time, arrival_time);
		if (!(edges.count(departure_planet) && edges[departure_planet].count(destination_planet))) {
			edges[departure_planet][destination_planet] = new Edge(destination_planet);
		}
		edges[departure_planet][destination_planet]->add(*leg); // here we finalize the edges
	}
	// once edges are finalized and legs are added to them. we can add them to the planets using the public add() method
	for (auto i : galaxy->planets) { for (auto k : galaxy->planets) { if (i != k) { if (edges.find(i) != edges.end()) { auto edge = edges[i].find(k); if (edge != edges[i].end()) { i->add(edge->second);}}}}}
	return true;
}

void Reader::Dijkstras() const {
	galaxy->search();
}

// ACTUALLY EXTRACTS THE CURRENT LINE FROM ROUTE.TXT YOOOOO. and puts ze shit in its correct place. ex. previous_arrival_time etc.
bool Reader::get_record() {
	if (getline(in, current_input_line)) {
		if (arrival_time == 0) {
			previous_ship_id = -1; // if this is our first record,,,,,, set previous ship id to -1
													  // this is so our validate shit runs properly. it knows if its a new route by comparing current ship id to previous
													  // currently, everything initializes to 0 (which is fine, yooo) but the start of the route the first ship may be
													  // id = 0 so sum shit would fail fosho, dig? we set it to -1 i know it's dirty but ah well.
		}
		
		else previous_ship_id = ship_id; // update previous ship id to the previous current ship cuz we about to read a new ship yo.
		previous_destination_planet = destination_planet; // update previous_destination_planet
		previous_arrival_time = arrival_time; // update previous_arrival_time
		
		
		string delimiter = "\t"; // tab delimiter because route.txt uses that in between entries
		size_t pos; // set initial pos (??) ok.
		auto i = 1; // counter for which variable to save current part of the line as. ex. each line has 5 pieces of information. we loop through the line
				   // we have to know which piece of information to save now
		while ((pos = current_input_line.find(delimiter)) != string::npos) { // loop for the file
			string token = current_input_line.substr(0, pos); // set our current token
			switch (i) { // based on which 5 pieces of information we're on now in the current line, that's what we save
			case 1:	ship_id = ships.find(token)->second; break;
			case 2: departure_planet = planets.find(token)->second; break;
			case 3: departure_time = stoi(token); break;
			case 4: destination_planet = planets.find(token)->second; break;
			default: ;
			}
			current_input_line.erase(0, pos + delimiter.length()); // move forward our token
			i++; // iterate i for our next piece of info from the line
		}
		arrival_time = stoi(current_input_line); // at the end our current line/string is axed so all 4 former pieces of information are already saved in their respective 
												 // correct locations and our fifth and final piece of info is all that's left of the (current_input_line) string so we save it.
		return true; // return true if this worked..
	}
	return false; // yup false if not. (the check is the getline, if no more lines, we done. / the false does that). :)
}

// TAKES THE CURRENT LINE FROM ROUTE.TXT AND RUNS THAT SHIT THRU A VIVID CHECK YOOOOO
bool Reader::validate() const {
	if (!(departure_time >= previous_arrival_time+4)) { // checks to make sure there's a correct layover!
		if (ship_id == previous_ship_id) { // if no layover (4 hours) detected. it must be a new ship then.. check
			cerr << "\nERROR: Current leg is not a valid continuation of the previous leg / is not the beginning of the route for another ship.\ninfo: " << endl;
			dump_current_info(); // dump for info
			return false; // we only got here if neither the above two are true. this false triggers our eventual EXIT_FAILURE in main..
		}
	}
	// CHECK MAKE SURE ROUTE.TXT TRAVEL TIME MATCHES CONDUITS.TXT TRAVEL TIME
	if (arrival_time - departure_time != constraints->transit_time(departure_planet->name, destination_planet->name)) {
		cerr << "\nERROR: Travel time does not seem right. Please step to the side for a random space search\ndebug info: " << endl;
		dump_current_info();
		return false;
	}
	cerr << "ALL GOOD:\n" << endl; // just a simple message all good after each entry 
	dump_current_info();
	return true;
}
