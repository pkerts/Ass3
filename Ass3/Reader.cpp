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
	Galaxy galaxy;

	auto planet_strings = constraints->planet_strings(); // TEST IF I CAN REMOVE THIS LINE AND REPLACE "planet_strings" with "constraints->planet_strings()"
	for (const auto& i : planet_strings) {
		auto planet = new Planet(i);
		galaxy.add(planet);
	}


	return nullptr;
}
