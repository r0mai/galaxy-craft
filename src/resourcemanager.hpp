
#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace gc {

class resourcemanager {
public:
	resourcemanager();

	const sf::Texture& get_texture(const std::string& s);
private:
	std::map<std::string, sf::Texture> texture_storage;

};
	
} // namespace gc

#endif //RESOURCEMANAGER_HPP_
