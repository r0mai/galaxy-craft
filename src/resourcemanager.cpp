
#include "resourcemanager.hpp"

#include <iostream>

namespace gc {

resourcemanager::resourcemanager(){

	sf::Image invalid_image;
	invalid_image.create( 50, 50, sf::Color(255, 20, 247) );

	texture_storage["invalid"].loadFromImage( invalid_image ); // invalid member added.
}

const sf::Texture& resourcemanager::get_texture(const std::string& s){
	// First, attempt to retrieve image!
	auto it = texture_storage.find(s);
	if( it != texture_storage.end() ){ // found!
		return it->second;
	}
	else{ // not found, attempt to open!
		sf::Texture img;
		if(img.loadFromFile(s)){ // can open!
			return texture_storage.insert(std::make_pair(s, img)).first->second;
		}
		else{
			std::cout << "Image manager failed to fetch file " << s << std::endl;
			return texture_storage.find("invalid")->second;
		}
	}
}

} //namespace gc
