#pragma once
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <optional>

enum BusType {
	forward,
	ring
};

struct Stop {
	std::string name;
	double longitude;
	double lattitude;
	std::unordered_map<std::string, int> distances;
};

struct Bus {
	std::string name;
	BusType type;
	std::vector<std::string> stops;
};

class BusHasher {
public:
    size_t operator() (const std::string& bus_name) const {
        return hasher_(bus_name);
     }
private:
    std::hash<std::string> hasher_;
};

class StopHasher {
public:
    size_t operator() (const std::string& stop_name) const {
        return hasher_(std::string(stop_name));
     }
private:
    std::hash<std::string> hasher_;
};

class TransportCatalogue {
public:
	TransportCatalogue() = default;
	void AddBus(Bus&);
	void AddStop(Stop&);
	int CalcUnicStops(std::string_view);
	int CalcStops(std::string_view);
	double CalcRouteCoordinateLength(std::string_view);
	int CalcRouteLength(std::string_view);
	bool IsBus(std::string_view);
	bool IsStop(std::string_view);
	std::set<std::string>& GetBuses(std::string_view);
	void FillDistancesBetweenStops();
private:
	double CalcRingRouteCoordinateLength(std::string_view);
	double CalcForwardRouteCoordinateLength(std::string_view);
	int CalcRingRouteLength(std::string_view);
	int CalcForwardRouteLength(std::string_view);
	std::unordered_map<std::string, Stop, StopHasher> stops_;
	std::unordered_map<std::string, Bus, BusHasher> buses_;
	std::unordered_map<std::string, std::set<std::string>> buses_by_stop_;
};