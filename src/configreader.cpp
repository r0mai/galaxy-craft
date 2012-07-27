
#include "configreader.hpp"

#include <fstream>

namespace gc {

configreader::configreader() {}

configreader::configreader(const std::string& file) {
	read_config(file);
}

void configreader::add_value(const std::string& key, const std::string& value) {
	//Could add DEBUG info, if a value gets overwritten
	config[key] = value;
}

void configreader::read_config(const std::string& file) {
	std::ifstream in(file);

	std::string line;
	while( std::getline( in, line ) ) {
		const unsigned separator = line.find( '=', 0 );
		if ( separator != std::string::npos ) {
			const std::string key = line.substr(0, separator);
			const std::string value = line.substr(separator+1);

			add_value(key, value);
		}
	}
}



} //namespace gc




