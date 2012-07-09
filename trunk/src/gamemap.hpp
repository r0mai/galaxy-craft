
#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "visilibity.hpp"

#include "drawable.hpp"
#include "path.hpp"
#include "polygon.hpp"


namespace gc {

class gamemap : public drawable {
public:
	static gamemap from_file(const std::string& file);

	path search_path(const vector2df& start, const vector2df& end) const;

	virtual void draw(sf::RenderWindow& window) const;

	const vector2df& get_dimension() const;
	const std::vector<polygonf>& get_obstacles() const;

private:
	vector2df dimension;
	std::vector<polygonf> obstacles;

	//Cached stuff
	VisiLibity::Environment vis_enviroment;
	VisiLibity::Visibility_Graph vis_visibility_graph;

	//initializes vis_enviroment and vis_visibility_graph
	//this has to be called whenever obstacles, or dimension is modified for correct path finding
	//calls init_enviroment() and init_visilibity_graph()
	void init_visilibity();

	void init_enviroment();
	void init_visilibity_graph();

	static const double vis_epsilon;
};

} //namespace gc

#endif /* MAP_HPP_ */
