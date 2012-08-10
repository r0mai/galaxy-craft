
#ifndef MINIMAP_HPP_
#define MINIMAP_HPP_

#include <boost/smart_ptr/shared_ptr.hpp>

#include <SFML/Graphics.hpp>

#include "vector2d.hpp"
#include "polygon.hpp"
#include "unit.hpp"

namespace gc {

class minimap {
public:
	minimap();

	minimap(const vector2df& original_map_size, const vector2df& position, const vector2di& draw_space_size);

	//Should be called only once, or when obstacles change
	void render_background(const std::vector<polygonf>& obstacles);

	void render_units(const std::vector<unit>& units);

	void draw(sf::RenderWindow& window) const;

private:

	vector2df original_map_size;
	vector2df position;
	vector2di draw_space_size;

	boost::shared_ptr<sf::RenderTexture> foreground_texture;
	boost::shared_ptr<sf::RenderTexture> background_texture;

	sf::Sprite foreground;
	sf::Sprite background;


};

} //namespace gc

#endif /* MINIMAP_HPP_ */
