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
	// TRAVEL TIMES
	string filename = argv[1];
	fstream in(filename);
	Travel_Times t;
	while (in >> t) {}
	t.dump();

	// READER
	Reader r(in, &t);

	RouteCreator c;
	c.Create(r, t);

	return 0;
}