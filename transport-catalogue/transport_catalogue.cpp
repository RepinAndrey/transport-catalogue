#include "transport_catalogue.h"
#include "geo.h"
#include <set>
#include <iostream>
#include <algorithm>
#include <execution>

using namespace std;

void TransportCatalogue::AddBus(Bus& bus) {
	buses_[bus.name] = bus;
	for(const auto s: bus.stops) {
		buses_by_stop_[s].insert(bus.name);
	}
}

void TransportCatalogue::AddStop(Stop& stop) {
	stops_[stop.name] = stop;
}

int TransportCatalogue::CalcUnicStops(string_view bus_name) {
	string name(bus_name);
	set<string> unic_stops;
	if(buses_.count(name)) {	
		for(const auto k : buses_[name].stops) {
			unic_stops.insert(k);
		}
	}
	return unic_stops.size();
}

int TransportCatalogue::CalcStops(string_view bus_name) {
	string name(bus_name);
	if(buses_.count(name)) {
		if (buses_[name].type == BusType::ring) {
			return buses_[name].stops.size();
		}
		else {
			return buses_[name].stops.size()*2 - 1;
		}
	}
	return 0;
}

bool TransportCatalogue::IsBus(string_view bus_name) {
	string name(bus_name);
	return buses_.count(name);
}

bool TransportCatalogue::IsStop(string_view stop_name) {
	string name(stop_name);
	return stops_.count(name);
}

double TransportCatalogue::CalcRingRouteCoordinateLength(string_view bus_name) {
	string name(bus_name);
	vector<string>::iterator it = buses_[name].stops.begin();
	double Sum = 0.0;
	while( it + 1 != buses_[name].stops.end()) {
		Coordinates crd1{stops_[*it].lattitude, stops_[*it].longitude};
		Coordinates crd2{stops_[*(it+1)].lattitude, stops_[*(it+1)].longitude};
		Sum += ComputeDistance(crd1, crd2);
		it++;
	}
	return Sum;
}

int TransportCatalogue::CalcRingRouteLength(string_view bus_name) {
	string name(bus_name);
	vector<string>::iterator it = buses_[name].stops.begin();
	int Sum = 0.0;
	while( it + 1 != buses_[name].stops.end()) {
		Sum += stops_[*it].distances[*(it+1)];
		it++;
	}
	return Sum;
}

double TransportCatalogue::CalcForwardRouteCoordinateLength(string_view bus_name) {
	string name(bus_name);
	vector<string>::iterator it = buses_[name].stops.begin();
	double Sum = 0.0;
	while( it + 1 != buses_[name].stops.end()) {
		Coordinates crd1{stops_[*it].lattitude, stops_[*it].longitude};
		Coordinates crd2{stops_[*(it+1)].lattitude, stops_[*(it+1)].longitude};
		Sum += ComputeDistance(crd1, crd2);
		it++;
	}
	while( it != buses_[name].stops.begin()) {
		Coordinates crd1{stops_[*it].lattitude, stops_[*it].longitude};
		Coordinates crd2{stops_[*(it-1)].lattitude, stops_[*(it-1)].longitude};
		double dist = ComputeDistance(crd1, crd2);
		Sum += dist;
		it--;
	}
	return Sum;
}

int TransportCatalogue::CalcForwardRouteLength(string_view bus_name) {
	string name(bus_name);
	vector<string>::iterator it = buses_[name].stops.begin();
	int Sum = 0.0;
	while( it + 1 != buses_[name].stops.end()) {
		Sum += stops_[*it].distances[*(it+1)];
		it++;
	}
	while( it != buses_[name].stops.begin()) {		
		Sum += stops_[*it].distances[*(it-1)];
		it--;
	}
	return Sum;
}

double TransportCatalogue::CalcRouteCoordinateLength(std::string_view bus_name) {
	double Sum = 0.0;
	if(buses_.count(string(bus_name))) {
		if (buses_[string(bus_name)].type == BusType::ring) {
			Sum = CalcRingRouteCoordinateLength(bus_name);
		}
		else {
			Sum = CalcForwardRouteCoordinateLength(bus_name);
		}
	}
	return Sum;
}

int TransportCatalogue::CalcRouteLength(std::string_view bus_name) {
	int Sum = 0.0;
	if(buses_.count(string(bus_name))) {
		if (buses_[string(bus_name)].type == BusType::ring) {
			Sum = CalcRingRouteLength(bus_name);
		}
		else {
			Sum = CalcForwardRouteLength(bus_name);
		}
	}
	return Sum;
}

set<string>& TransportCatalogue::GetBuses(string_view stop_name) {
	string name(stop_name);
	static set<string> st;
	if(buses_by_stop_.count(string(stop_name))) {
		return buses_by_stop_[name];
	}
	else {
		return st;
	}
}

void TransportCatalogue::FillDistancesBetweenStops() {
	for( const auto [k,v] : stops_) {
		for( const auto [lk, lv] : v.distances) {
			if(!stops_.at(lk).distances.count(k)) {
				stops_.at(lk).distances[k] = lv;
			}
		}
	}
}