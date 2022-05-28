#pragma once
#include <string_view>


#include "transport_catalogue.h"

void ParseStop(std::string_view, Stop&);

void ParseBus(std::string_view, Bus&);