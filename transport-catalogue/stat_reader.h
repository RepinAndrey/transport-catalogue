#pragma once
#include <string_view>
#include <tuple>

#include "transport_catalogue.h"

enum StatType {
	StatBus,
	StatStop
};

std::pair<StatType, std::string_view> ParseStat(std::string_view);
