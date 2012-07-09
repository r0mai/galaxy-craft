
#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace gc {

class resourcemanager {
public:
	resourcemanager();

	const sf::Image& getimage(const std::string& s);
private:
	std::map<std::string, sf::Image> image_storage;

};
	
} // namespace gc

#endif //RESOURCEMANAGER_HPP_
