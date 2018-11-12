// galaxy.h
//
// Declarations for a graph representing Old Republic Spaceways' route
// structure.  Nodes are system planets edges hold the list of ships'
// legs traveling from the origin to destination planet.
//
// Additional fields are defined to allow implementation of Dijkstra's
// algorithm to find the minimum cost (earliest arrival time) between
// pairs of planets.
//
// Copyright 2013, 2018 Systems Deployment, LLC
// Author: Morris Bernstein (morris@systems-deployment.com)

#if !defined(GALAXY_H)
#define GALAXY_H

#include <climits>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "priority.h"
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>

typedef int Time;
const Time MAX_TIME = INT_MAX;
const Time TURNAROUND_TIME = 4;
const Time TRANSFER_TIME = 4;

typedef int Ship_ID;

class Planet;
class Galaxy;

class Travel_Times {
public:
	void add(const std::string& origin, const std::string& destination, const std::string& time) {times[origin][destination] = std::stoi(time);
																								  times[destination][origin] = std::stoi(time);
																								  planet_names.insert(origin);
																								  planet_names.insert(destination);	}
	// void dump() {for (const auto& i : times) {for (const auto& k : i.second) {std::cerr << i.first << '\t' << k.first << '\t' << k.second << '\n';}}}
	void dump() { for (const auto& i : times) { std::cerr << i.first << '\n'; for (const auto& k : i.second) { std::cerr << "\t\t" << k.first << '\t' << k.second << '\n'; } } }

	std::map<std::basic_string<char>, std::map<std::basic_string<char>, int>>::const_iterator
	immediate_neighbors(const std::string& planet) const { return times.find(planet); }

	Time transit_time(std::string origin, std::string destination) {
		return times.find(origin)->second.find(destination)->second;
	}

	int obtain_origin_int(std::string origin_name) const {
		int i = 1;
		for (const auto& k : times) {
			if (k.first == origin_name) return i;
			i++;
		}
		return 0;
	}

	int obtain_destination_int(int origin_int, std::string destination_name) const {
		int i = 1;
		for (const auto& k : times) {
			if (i == origin_int) {
				i = 1;
				for (const auto& j : k.second) {
					if (j.first == destination_name) return i;
					i++;
				}
			}
		}
		return 0;
	}

	std::string dump(int number) const {
		int i = 1;
		if (number == 0) {
			for (const auto& k : times) {
				std::cout << i << " " << k.first << std::endl;
				i++;
			}
			return "";
		}
		for (const auto& k : times) {
			if (number == i) {
				i = 1;
				for (const auto& j : k.second) {
					std::cout << i << " " << j.first << std::endl;
					i++;
				}
				return k.first;
			}
			i++;
		}
		return "END OF STD::STRING DUMP(INT NUMBER) CONST";
	}

	std::pair<std::string, int> travel_time(int origin, int destination) const {
		int counter = 1;
		for (const auto& i : times) {
			if (counter == origin) {
				counter = 1;
				for (const auto& j : i.second) {
					if (counter == destination) {
						return j;
					}
					counter++;
				}
			}
			counter++;
		}
		return std::make_pair("", 0);
	}
	std::set<std::string>::iterator begin() { return planet_names.begin(); }
	std::set<std::string>::iterator end() { return planet_names.end(); }
private:
	std::set<std::string> planet_names; // Should I just make this public as opposed to the above vector? or what?
	std::map<std::string, std::map<std::string, Time> > times;
};

// Class Fleet maps internal ship ID to the ship's name .
class Fleet {
public:
  Ship_ID add(const std::string& name) {names.push_back(name); return names.size() - 1;}
  const std::string& name(Ship_ID id) const {return names[id];}

private:
  std::vector<std::string> names;
};


// Class Leg represents a single leg of an itinerary, consisting of a
// ship ID, departure time, and arrival time.  Legs are associated
// with an edge between two planets (vertices) in the galaxy map.
//
// A pair of legs may be compared to find the earliest arrival time.
class Leg {
public:
  Leg(): id(-1), departure_time(MAX_TIME), arrival_time(MAX_TIME) {}
  Leg(Ship_ID id, Time departure_time, Time arrival_time)
    : id(id), departure_time(departure_time), arrival_time(arrival_time) {
  }

  // Return negative, zero, or positive for left leg arriving before,
  // same time, or after the right leg (respectively
  static int compare(const Leg& left, const Leg& right) {
    return left.arrival_time - right.arrival_time;
  }

  static bool less_than(const Leg& left, const Leg& right) {
    return compare(left, right) < 0;
  } 

  Ship_ID id;
  Time departure_time;
  Time arrival_time;
};


// Class Itinerary is a sequence of legs with a parallel sequence of
// destination planets. i.e. destinations[i] is the destination of
// leg[i].
class Itinerary {
public:
  Itinerary(Planet* origin): origin(origin) {}
  void print(Fleet& fleet, std::ostream& out=std::cout);

  Planet* origin;
  std::vector<Planet*> destinations;
  std::vector<Leg> legs;
};


// Class Edge is a single edge in the route graph.  It consists of a
// destination planet and a sequence of legs departing from the origin
// planet (vertex) to the destination planet.
class Edge {
public:
  Edge(Planet* destination): destination(destination) {}
  void add(Leg& leg) {departures.push_back(leg);}

  // sort(): sort the legs of this edge by arrival time to the
  // destination planet.
  void sort();

  void dump(Galaxy* galaxy);

  Planet* destination;
  std::vector<Leg> departures;
};


//  Class Planet is a node in the route graph.  It contains a sequence
//  of edges plus additional fields to allow implementation of
//  Dijkstra's shortest-path algorithm.
class Planet {
public:
  Planet(const std::string& name): name(name) {}
  void add(Edge* e) {edges.push_back(e);}

  // reset() clears the fields set by Dijkstra's algorithm so the
  // algorithm may be re-run with a different origin planet.
  void reset() {predecessor = nullptr; best_leg = Leg();}

  // search() computes the shortest path from the Planet to each of the
  // other planets and returns the furthest planet by travel time.
  Planet* search(PriorityQueue<Planet, int (*)(Planet*, Planet*)>& queue);

  // make_itinerary() builds the itinerary with the earliest arrival
  // time from this planet to the given destination planet.
  Itinerary* make_itinerary(Planet* destination);

  // arrival_time() is the time to arrive at this planet from the
  // origin planet that was used to compute the most recent search().
  Time arrival_time() const {return best_leg.arrival_time;}

  // Debug-friendly output.
  void dump(Galaxy* galaxy);

  // Functions for priority queue:
  int get_priority() {return priority;}
  void set_priority(int new_priority) {priority = new_priority;}
  static int compare(Planet* left, Planet* right) {
    return Leg::compare(left->best_leg, right->best_leg);
  }

  const std::string name;

private:
  // relax_neighbors(): for each neighboring planet of this planet,
  // determine if the route to the neighbor via this planet is faster
  // than the previously-recorded travel time to the neighbor.
  void relax_neighbors(PriorityQueue<Planet, int (*)(Planet*, Planet*)>& queue);

  // edges shows the connections between this planet and it's
  // neighbors.  See class Edge.
  std::vector<Edge*> edges;

  // For Dijkstra's algorithm:
  Planet* predecessor;
  Leg best_leg;
  int priority;
};


// Class galaxy holds the graph of Old Republic Spaceways' route
// structure, consisting of a sequence of planets (vertices).  The
// graph is constructed by adding new planets to the Galaxy object and
// adding edges to the planet objects.
class Galaxy {
public:
  void add(Planet * planet) {planets.push_back(planet);}

  void reset() {for (auto planet: planets) {planet->reset();}}

  // For each planet, apply Dijkstra's algorithm to find the minimum
  // travel time to the other planets.  Print the itinerary to the
  // furthest planet. Terminate with EXIT_FAILURE if the graph is not
  // strongly connnected (you can't get there from here).  Finally,
  // print the diameter of the galaxy and its itinerary.
  void search();

  void dump();
  void dump_routes(Planet* origin, std::ostream& out=std::cerr);

  Fleet fleet;
  std::vector<Planet*> planets;
};

inline void Edge::sort() {
	std::sort(departures.begin(), departures.end(), Leg::compare);
}

inline Planet* Planet::search(PriorityQueue<Planet, int(*)(Planet*, Planet*)>& queue) {
	// for (auto p : )
	
	best_leg.departure_time = 0;
	best_leg.arrival_time = 0;

	std::vector<Planet*> neighbors;
	for (auto i : edges) {
		i->destination->predecessor = this;
		i->sort();
		neighbors.push_back(i->destination);
	}

	for (auto v : neighbors) {
		queue.push_back(v);
	}

	auto u = queue.pop();
	
	//while (!queue.empty()) {
	//	auto u = queue.pop();
	//	std::vector<Planet*> neighbors_of_u;
	//	for (auto vu : u->edges) {
	//		neighbors_of_u.push_back(vu->destination);
	//	}
	//	for (auto neighbor_v : neighbors_of_u) {
	//		// auto alt = 
	//	}
	//}

	for (auto i : edges) {
		;
	}

	return u;
}

inline Itinerary* Planet::make_itinerary(Planet* destination) {
	best_leg.departure_time = 0;
	best_leg.arrival_time = 0;

	PriorityQueue<Planet, int(*)(Planet*, Planet*)> pq(compare);
	
}

inline void Galaxy::search() {
	// itinerary furthest galaxy planet
	for (auto k : planets) {
		Itinerary* farthest_planet{};
		for (auto i : planets) {
			if (i != k) {
				reset();
				PriorityQueue<Planet, int(*)(Planet*, Planet*)> pq(Planet::compare);
				auto itinerary = k->make_itinerary(i);
				if (!farthest_planet) {
					farthest_planet = itinerary;
				}
				else {
					auto total_time = itinerary->legs.back().arrival_time - itinerary->legs.front().departure_time;
					if (total_time > farthest_planet->legs.back().arrival_time - farthest_planet->legs.front().departure_time) {
						farthest_planet = itinerary;
					}
				}
				// just finished writing the above 10:35 AM
				// the below line probably needs to go
				i->search(pq);

				// Now we just finish in make_itinerary. it will do the starting steps and now we must search shortest time to the planet passed to make itinerary
				// using planet search. hmmmm. but the planet search does not take a planet pointer
			}
		}
		// dump furthest planet
		// initialize and check against furthest galaxy planet
	}
}

#endif
