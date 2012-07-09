#ifndef IMANAGER_HPP
#define IMANAGER_HPP
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace gc{

class imagemanager{
public:
	const sf::Image& getimage(const std::string& s);
	imagemanager();
private:
	std::map<std::string, sf::Image> storage;

};
	
}// namespace gc
#endif