
#include "minimap.hpp"

#include <algorithm>

namespace gc {

minimap::minimap() {}

minimap::minimap(const vector2df& original_map_size, const vector2df& position, const vector2di& draw_space_size) :
		original_map_size(original_map_size), position(position), draw_space_size(draw_space_size),
		foreground_texture(new sf::RenderTexture), background_texture(new sf::RenderTexture)
{
	background_texture->create( draw_space_size.x, draw_space_size.y );
	foreground_texture->create( draw_space_size.x, draw_space_size.y );
}



void minimap::render_background(const std::vector<polygonf>& obstacles) {

	background_texture->clear( sf::Color::Blue );

	const vector2df multiplier = vector2df(draw_space_size) / original_map_size;

	std::for_each( obstacles.begin(), obstacles.end(), [this, &multiplier](polygonf poly) {
		poly.scale( multiplier );
		poly.rerender();
		background_texture->draw(poly);
	});

	background_texture->display();
	background = sf::Sprite(background_texture->getTexture());

	background.setPosition( position.to_sfml_vector() );
}

void minimap::render_foreground(const std::vector<unit>& units, const sf::View& view) {

	foreground_texture->clear( sf::Color::Transparent );

	const vector2df multiplier = vector2df(draw_space_size) / original_map_size;

	const vector2df blip_size = sf::Vector2f(3.f, 3.f);

	std::for_each( units.begin(), units.end(), [this, &multiplier, &foreground_texture, &blip_size](const unit& u) {
		sf::RectangleShape blip( blip_size.to_sfml_vector() );
		blip.setFillColor( sf::Color::Black );
		blip.setPosition( (u.get_position() * multiplier - blip_size/2.f).to_sfml_vector() );
		foreground_texture->draw(blip);
	});

	const vector2df scaled_view_size = vector2df(view.getSize()) * multiplier;
	const vector2df scaled_center = vector2df(view.getCenter()) * multiplier;

	sf::RectangleShape view_rectangle( scaled_view_size.to_sfml_vector() );
	view_rectangle.setOutlineColor( sf::Color::White );
	view_rectangle.setOutlineThickness(1.f);
	view_rectangle.setFillColor( sf::Color::Transparent );
	view_rectangle.setPosition( (scaled_center - scaled_view_size/2.f).to_sfml_vector() );

	foreground_texture->draw( view_rectangle );

	foreground_texture->display();

	foreground = sf::Sprite(foreground_texture->getTexture());

	foreground.setPosition( position.to_sfml_vector() );
}

void minimap::draw(sf::RenderTarget& window, sf::RenderStates states) const {
	window.draw( background, states );
	window.draw( foreground, states );
}

} //namespace gc
