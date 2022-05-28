#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <vector>
#include <tuple>

using namespace std;

 void Load(vector<string>& inp_queries, vector<string>& stat_queries) {
	ifstream file("example.txt");
	string line;
	bool inp = false;
	int inp_lines = 0;
	int stat_lines = 0;
    int stat_counter = 0; 
	while(std::getline(std::cin, line)/*getline(file, line)*/) {
		if(isdigit(line[0])) {
			inp = !inp;
			if(inp) {
				inp_lines = stoi(line);
				inp_queries.reserve(inp_lines);
			}
			else {
				stat_lines = stoi(line);
				stat_queries.reserve(stat_lines);
			} 
			continue;
		}
		if(inp) {
			inp_queries.push_back(line);
		}
		else {
			stat_queries.push_back(line);
            stat_counter++;
            if(stat_counter >= stat_lines) break;
		}
	}
}

void LoadInpQueries(const vector<string> & inp_queries, TransportCatalogue& catalogue) {
	for(const auto line : inp_queries) {
		if(line[0] == 'S') {
			Stop stop;
			ParseStop(line, stop);
			catalogue.AddStop(stop);
		}
		else if(line[0] == 'B') {
			Bus bus;
			ParseBus(line, bus);
			catalogue.AddBus(bus);
		}
	}
	catalogue.FillDistancesBetweenStops();
}

void PrintBuses(TransportCatalogue& catalogue, string_view bus_name) {
	if(catalogue.IsBus(bus_name)) {
		int dist = catalogue.CalcRouteLength(bus_name);
		double curv = (double)dist/catalogue.CalcRouteCoordinateLength(bus_name);
		cout << "Bus " <<  bus_name << ": " << catalogue.CalcStops(bus_name) << " stops on route, "
		<< catalogue.CalcUnicStops(bus_name) << " unique stops, " << 
		dist << " route length, " << curv << " curvature" << endl;			
	}
	else {
		cout << "Bus " <<  bus_name << ": not found" << endl;
	}
}

void PrintStops(TransportCatalogue& catalogue, string_view stop_name) {
	if(catalogue.IsStop(stop_name)) {
		set<string> st = catalogue.GetBuses(stop_name);
		if(st.empty()) {
			cout << "Stop " <<  stop_name << ": no buses" << endl; 
		}
		else {
			cout << "Stop " <<  stop_name << ": buses ";
			auto it = st.begin();
			for(;;)
			{
				cout << *it;
				if(++it == st.end()) break;
				cout << " ";
			}
			cout << endl;
		}
			
	}
	else {
		cout << "Stop " <<  stop_name << ": not found" << endl;
	}
}

void PrintStat(const vector<string>& stat_queries, TransportCatalogue& catalogue) {
	for(const auto line : stat_queries) {
		const auto [stat_type, name] = ParseStat(string_view(line));
		if(stat_type == StatBus) {
			PrintBuses(catalogue, name);
		}
		else if(stat_type == StatStop) {
			PrintStops(catalogue, name);
		}
	}
}

int main()
{
	TransportCatalogue catalogue;
	vector<string> inp_queries;
	vector<string> stat_queries;
	Load(inp_queries, stat_queries);
	LoadInpQueries(inp_queries, catalogue);
	PrintStat(stat_queries, catalogue);
	return 0;
}