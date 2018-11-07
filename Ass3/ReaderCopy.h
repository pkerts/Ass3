#pragma once
#include <istream>
#include "galaxy.h"
#include <map>



class Reader {
public:


private:
	static const int MIN_LAYOVER_TIME;

	// Verify that that current leg is a valid continuation of the
	// previous leg or the beginning of the route for another ship.
	bool validate();


	// Planet-name pair to edge object
	std::map<const Planet*, std::map<const Planet*, Edge*> > edges;
};
