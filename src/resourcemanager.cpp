
#include "resourcemanager.hpp"

#include <iostream>

namespace gc {

resourcemanager::resourcemanager(){
	image_storage["invalid"] = sf::Image( 50, 50, sf::Color(255, 20, 247) ); // invalid member added.
}

const sf::Image& resourcemanager::getimage(const std::string& s){
	// First, attempt to retrieve image!
	auto it = image_storage.find(s);
	if( it != image_storage.end() ){ // found!
		return it->second;
	}
	else{ // not found, attempt to open!
		sf::Image img;
		if(img.LoadFromFile(s)){ // can open!
			return image_storage.insert(std::pair<std::string, sf::Image>(s, img)).first->second;
		}
		else{
			std::cout<<"Image manager failed to fetch file "<<s<<std::endl;
			return image_storage.find("invalid")->second;
		}
	}
}

} //namespace gc
