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
