#include "galaxy.h"
#include <fstream>
#include <string>
#include <vector>
#include "Reader.h"

using namespace std;


struct Data {
	string name;
};

vector<int> times;

istream& operator>>(istream& is, Travel_Times& t) {
	string origin;
	string destination;
	string time;
	if (getline(is, origin, '\t') &&
		getline(is, destination, '\t') &&
		getline(is, time)) {
		t.add(origin, destination, time);
	}
	return is;
}

int main(int argc, char** argv) {
	const string filename = argv[1]; // argument passed. this is route.txt
	fstream in(filename); // fstream created w/ route.txt


	// TRAVEL TIMES
	fstream conduits("conduits.txt"); // fstream for conduits.txt
	Travel_Times t; // TravelTimes object created
	while (conduits >> t) {} // Fill TravelTimes w conduits.txt
	t.dump(); // dump it for validation (std::cerr)


	// READER
	Reader r(in, &t); // create a reader w/ route.txt and Travel_Times which we filled with "conduits.txt"

	// TO USE ROUTE CREATOR, (1/2) COMMENT OUT THIS BLOCK (the following four lines)
	if (!r.ReadAndVerify()) {
		// 
		return EXIT_FAILURE;
	}
	r.Dijkstras();

	// CREATOR. TO USE ROUTE CREATOR, (2/2) UNCOMMENT THIS BLOCK (the following two lines)
	// RouteCreator c;
	// c.Create(r, t);
	return 0;
}
