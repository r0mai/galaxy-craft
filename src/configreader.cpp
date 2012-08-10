
#include "configreader.hpp"

#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>

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

		boost::trim( line );

		if ( line.empty() || line[0] == '#' ) { //empty line, or comment
			continue;
		}

		const unsigned separator = line.find( '=', 0 );
		if ( separator != std::string::npos ) {
			std::string key = line.substr(0, separator);
			std::string value = line.substr(separator+1);

			boost::trim( key );
			boost::trim( value );

			if ( key == "$include" ) {
				std::cout << "reading file=\"" << value << "\"\n";
				read_config(value); //TODO infinite recursion possible : check if we've read this file already
			} else {
				std::cout << "key=\"" << key << "\", \"" << value << "\"\n";
				add_value(key, value);
			}
		}
	}
}



} //namespace gc




