#include "imanager.hpp"

namespace gc{



imagemanager::imagemanager(){
	storage.insert(std::pair<std::string, sf::Image>("invalid", sf::Image( 50, 50, sf::Color(255, 20, 247) ) )); // invalid member added.
}

const sf::Image& imagemanager::getimage(const std::string& s){
	// First, attempt to retrieve image!
	auto it = storage.find(s);
	if( it != storage.end() ){ // found!
		return it->second;
	}
	else{ // not found, attempt to open!
		sf::Image img;
		if(!img.LoadFromFile(s)){ // can open!
			storage.insert(std::pair<std::string, sf::Image>(s, img));
		}
		else{
			std::cout<<"Image manager failed to fetch file "<<s<<std::endl;
			return storage.find("invalid")->second;
		}
	}
}

}