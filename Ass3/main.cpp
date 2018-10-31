#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Data {
	string city;
	string state;
	string zipCode;
};

vector<string> A;
vector<string> B;
vector<int> times;

istream& operator>>(istream& is, Data& d) {
	string origin;
	string dest;
	string time;
	// int travel_time;
	if (getline(is, origin, '\t') &&
		getline(is, dest, '\t') &&
		getline(is, time)) {
		// d.zipCode = stoi(zipcode);
		// COMMENTED OUT
		// cout << origin << '\t' << dest << '\t' << time << '\n';
		A.push_back(origin);
		B.push_back(dest);
		times.push_back(stoi(time));
	}

	return is;
}

int main(int argc, char** argv) {
	string filename = argv[1];
	fstream in(filename);

	Data d;
	// string size;
	// getline(in, size);
	// getline(in, size);

	while (in >> d) {
		// cout << "Yes! It worked!";
		;
	}

	for (size_t i = 0; i < A.size(); i++) {
		cout << A[i] << '\t' << B[i] << '\t' << times[i] << '\n';
	}

	// cout << "\n" << d.city << " " << d.state << " " << d.zipCode << endl;

	/*cout << "hello" << endl;

	string line;
	while (getline(in, line)) {
		istringstream iss(line);
		string a, b, c;
		int d, e;
		iss >> a >> b >> d >> c >> e;
		cout << a << " " << b << " " << d << " " << c << " " << e << endl;
	}*/

	return 0;
}