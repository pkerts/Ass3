#include "galaxy.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "Reader.h"
#include <set>

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
			Ab.insert(origin);
			A.push_back(origin);
			B.push_back(destination);
			Bb.insert(destination);
			times.push_back(stoi(time));
	}

	return is;
}

int main(int argc, char** argv) {
	// TRAVEL TIMES
	string filename = argv[1];
	fstream in(filename);
	Travel_Times t;
	while (in >> t) {}
	t.dump();

	/*Reader r(in, &t);
	Fleet f;
	ifstream ships("ship-names.txt");
	string ship;
	while (getline(ships, ship)) {
		f.add()
	}*/

/*
	for (auto i = 0; i < A.size(); i++) {
		cout << A[i] << '\t' << B[i] << '\t' << times[i] << '\n';
	}*/

	/*for (auto i : Travel_Times) {
		cout << i.first.first << '\t' << i.first.second << '\t' << i.second << '\n';
	}*/

	return 0;
}