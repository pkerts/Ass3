#include "galaxy.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "Reader.h"
#include <set>
#include "route_creator.h"

using namespace std;


struct Data {
	string name;
};

set<string> Ab;
set<string> Bb;
vector<string> A;
vector<string> B;
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


			/*Ab.insert(origin);
			A.push_back(origin);
			B.push_back(destination);
			Bb.insert(destination);
			times.push_back(stoi(time));*/
	

	return is;
}

int main(int argc, char** argv) {
	string filename = argv[1]; // argument passed. this is route.txt
	fstream in(filename); // fstream created w/ route.txt


	// TRAVEL TIMES
	fstream conduits("conduits.txt"); // fstream for conduits.txt
	Travel_Times t; // TravelTimes object created
	while (conduits >> t) {} // Fill TravelTimes w conduits.txt
	t.dump(); // dump it for validation (std::cerr)


	// READER
	Reader r(in, &t); // create a reader w/ route.txt and Travel_Times which we filled with "conduits.txt"

	// TO USE ROUTE CREATOR, COMMENT OUT THIS BLOCK (the following three lines)
	if (!r.ReadAndVerify()) { // 
		return EXIT_FAILURE;
	}

	r.Dijkstras();

	// CREATOR. TO USE ROUTE CREATOR, UNCOMMENT THIS BLOCK
	//RouteCreator c;
	//c.Create(r, t);
	return 0;
}