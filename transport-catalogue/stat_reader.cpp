#include "stat_reader.h"

#include <iostream>

using namespace std;

pair<StatType, string_view> ParseStat(string_view query) {
	StatType type = StatBus;
	if(query[0] == 'S') type = StatStop;
	
	if(type == StatType::StatBus) {
		query.remove_prefix(4);
	}
	else {
		query.remove_prefix(5);
	}
	return make_pair(type, query);
}




