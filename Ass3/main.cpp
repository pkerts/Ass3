#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

int main(int argc, char** argv) {
	string filename = argv[1];
	fstream in(filename);

	cout << "hello" << endl;
	return 0;
}