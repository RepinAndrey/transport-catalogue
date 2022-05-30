#include "input_reader.h"
#include <string>
#include <sstream>
#include <tuple>

#include <iostream>

using namespace std;

tuple<string, int> ParseDistance(string_view line) {
	size_t delim_pos = line.find("m");
	int dist = stoi(string(line.substr(0, delim_pos)));
	line.remove_prefix(delim_pos + 5);
	string stop = string(line.substr(0, line.size()));
	return make_pair(stop, dist);
}

void ParseStop(string_view line, Stop& stop, string& name) {
	size_t first = line.find(" ");
	size_t second = line.find(":");
	name = line.substr(first + 1, second - first - 1);
	line.remove_prefix(second + 1);
		
	size_t pos = 0;
	std::string token;
	int counter = 1;
	do {
		pos = line.find(',');
		if(pos == string::npos) {
			token = line;
		}
		else {
			token = line.substr(0, pos);
			line.remove_prefix(pos + 1);
		}
		if(counter == 1) {
			stop.lattitude = stod(token);
			counter++;
		}
		else if(counter == 2) {
			stop.longitude = stod(token);
			counter++;
		}
		else {
			auto [k, v] = ParseDistance(token);
			stop.distances[k] = v;
		}
	} while( pos != string::npos);
}

void ParseBus(string_view line, Bus& bus, string& name) {
	size_t first = line.find(" ");
	size_t second = line.find(":");
	size_t third = line.find(">");
	
	string strBusType;
	name = string(line.substr(first + 1, second - first - 1));
	line.remove_prefix(second + 1);
	if(third != line.npos) {
		bus.type = BusType::ring;
		strBusType = ">";
	}
	else {
		bus.type = BusType::forward;
		strBusType = "-";
	}
	third = line.find(strBusType);
	while(third != line.npos) {
		string_view stop_name = line.substr(0, third);
		first = stop_name.find_first_not_of(" ");
		second = stop_name.find_last_not_of(" ");
		stop_name = stop_name.substr(first, second - first + 1);
		bus.stops.push_back(string(stop_name));
		line.remove_prefix(third + 1);
		third = line.find(strBusType);		
	}
	if(!line.empty()) {
		first = line.find_first_not_of(" ");
		second = line.find_last_not_of(" ");
		line = line.substr(first, second - first + 1);
		bus.stops.push_back(string(line));
	}
}