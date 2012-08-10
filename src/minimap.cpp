
#include "minimap.hpp"

#include <algorithm>

namespace gc {

minimap::minimap() {}

minimap::minimap(const vector2df& original_map_size, const vector2df& position, const vector2di& draw_space_size) :
		original_map_size(original_map_size), position(position), draw_space_size(draw_space_size),
		foreground_texture(new sf::RenderTexture), background_texture(new sf::RenderTexture) {}



void minimap::render_background(const std::vector<polygonf>& obstacles) {

	background_texture->create( draw_space_size.x, draw_space_size.y );
	background_texture->clear( sf::Color::Blue );
	background_texture->display();
	background = sf::Sprite(background_texture->getTexture());

	background.setPosition( position.to_sfml_vector() );
}

void minimap::render_units(const std::vector<unit>& units) {

	foreground_texture->create( draw_space_size.x, draw_space_size.y );

	foreground_texture->clear( sf::Color::Transparent );

	const vector2df multiplier = vector2df(draw_space_size) / original_map_size;

	const vector2df blip_size = sf::Vector2f(3.f, 3.f);

	std::for_each( units.begin(), units.end(), [this, &multiplier, &foreground_texture, &blip_size](const unit& u) {
		sf::RectangleShape blip( blip_size.to_sfml_vector() );
		blip.setFillColor( sf::Color::Black );
		blip.setPosition( (u.get_position() * multiplier - blip_size/2.f).to_sfml_vector() );
		foreground_texture->draw(blip);
	});
	foreground_texture->display();

	foreground = sf::Sprite(foreground_texture->getTexture());

	foreground.setPosition( position.to_sfml_vector() );
}

void minimap::draw(sf::RenderWindow& window) const {


	window.draw( background );
	window.draw( foreground );
}

} //namespace gc
