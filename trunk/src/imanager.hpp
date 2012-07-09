#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_
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

}// namespace gc




#endif