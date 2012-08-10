

#ifndef CONFIGREADER_HPP_
#define CONFIGREADER_HPP_

#include <map>
#include <stdexcept>
#include <string>

#include <boost/lexical_cast.hpp>

namespace gc {

class configreader {
public:
	configreader();
	configreader(const std::string& file);


	void add_value(const std::string& key, const std::string& value);

	template<class T>
	T get_value(const std::string& key) const;

	void read_config(const std::string& file);
private:
	std::map<std::string, std::string> config;
};


template<class T>
T configreader::get_value(const std::string& key) const {
	auto it = config.find(key);
	if ( it == config.end() ) {
		throw std::invalid_argument("\"" + key + "\" doesn't exist in config");
	} else {
		try {
			return boost::lexical_cast<T>( it->second );
		} catch ( const boost::bad_lexical_cast& ) {
			throw std::invalid_argument("The value for \"" + key + "\" is not convertible to desired type");
		}
	}
}

} //namespace gc

#endif /* CONFIGREADER_HPP_ */
