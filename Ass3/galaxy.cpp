#include "galaxy.h"

void Planet::relax_neighbors(PriorityQueue<Planet, int(*)(Planet*, Planet*)>& queue) {
	for (auto edge : edges) {
		edge->sort();
		for (const auto departure : edge->departures) {
			// Checking constraints
			if (arrival_time() == 0 || departure.departure_time >= arrival_time() + TRANSFER_TIME) {
				// If a shorter route is detected
				if (departure.arrival_time < edge->destination->arrival_time()) {
					// Update
					edge->destination->best_leg = departure;
					edge->destination->predecessor = this;
					queue.reduce(edge->destination);
				}
			}
		}
	}
}

void Itinerary::print(Fleet& fleet, std::ostream& out) {
	if (!destinations.empty()) {
		// Header
		out << "///////////////////////////////////////////////////////////////////" << std::endl;
		out << std::uppercase << origin->name << std::nouppercase << std::endl << "\n\n";
		out << origin->name << " itinerary to furthest planet:\n\n";

		// Main meat
		out << fleet.name(legs.back().id) << '\t' << origin->name << '\t' << legs.back().departure_time << '\t' <<
			destinations.back()->name << '\t' << legs.back().arrival_time << std::endl;
		auto og = destinations.back()->name; // Save necessary name before popping

		legs.pop_back(); // moving backwards
		destinations.pop_back(); // until we reach origin

		// Keep going and repeating/printing until we reach origin and are done
		while (!destinations.empty()) {
			out << fleet.name(legs.back().id) << '\t' << og << '\t' << legs.back().departure_time << '\t' <<
				destinations.back()->name << '\t' << legs.back().arrival_time << std::endl;
			og = destinations.back()->name;
			legs.pop_back();
			destinations.pop_back();
			if (destinations.empty()) {
				out << "\n-->" << std::uppercase << og << std::nouppercase << std::endl;
			}
		}
		out << "\n\n" << std::endl;
	}
}

void Edge::sort() {
	std::sort(departures.begin(), departures.end(), Leg::less_than);
}

Planet* Planet::search(PriorityQueue<Planet, int(*)(Planet*, Planet*)>& queue) {
	// Set origin to 0
	best_leg.departure_time = 0;
	best_leg.arrival_time = 0;
	queue.reduce(this);

	// Create and initialize variable to track and store furthest planet
	auto furthest_planet = this;

	while (!queue.empty()) {
		// Pop next planet and check to make sure we have a workable value, if not, graph is inadequate and return an error message
		auto min = queue.pop();
		if (min->arrival_time() == INT_MAX) {
			std::cerr << "Graph not adequate." << std::endl;
			return nullptr;
		}
		min->relax_neighbors(queue);
		// Update furthest planet
		if (min->arrival_time() > furthest_planet->arrival_time()) {
			furthest_planet = min;
		}
	}
	return furthest_planet;
}

Itinerary* Planet::make_itinerary(Planet* destination) {
	auto* itinerary = new Itinerary(this);

	// Build the itinerary
	auto current = destination;
	// Trace back the route using the predecessor from the destination until we reach the origin
	while (current->predecessor) {
		itinerary->destinations.push_back(current);
		itinerary->legs.push_back(current->best_leg);
		current = current->predecessor;
	}

	return itinerary;
}

void Galaxy::search() {
	assert(!planets.empty());

	Time diameter_arrival_time{}; // Save the diameter arrival time so we can compare it in between resets
	Itinerary* diameter_itinerary{};

	for (auto i : planets) {
		// For each planet, reset then create and fill a priority queue
		reset();
		PriorityQueue<Planet, int(*)(Planet*, Planet*)> pq(Planet::compare);
		for (auto k : planets) { pq.push_back(k); }

		const auto furthest_planet = i->search(pq); // Search

		// Print itinerary
		auto itin = i->make_itinerary(furthest_planet);
		itin->print(fleet);
		delete itin;

		// Check against furthest planet arrival time and update diameter if needed
		if (furthest_planet->arrival_time() > diameter_arrival_time) {
			diameter_arrival_time = furthest_planet->arrival_time();
			delete diameter_itinerary;
			diameter_itinerary = i->make_itinerary(furthest_planet);
		}
	}

	std::cout << "\nWeighted diameter:" << std::endl;
	diameter_itinerary->print(fleet);
	delete diameter_itinerary;
}