
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
	//obstacle_offset : The distance between the original obstacles,
	//and the little bit larger polygon enclosing it, which is used for path searching
	static gamemap from_file(const std::string& file, float obstacle_offset);

	path search_path(const vector2df& start, const vector2df& end) const;

	virtual void draw(sf::RenderTarget& window) const;

	const vector2df& get_dimension() const;
	const std::vector<polygonf>& get_obstacles() const;

	const VisiLibity::Environment& get_vis_enviroment() const;
	const VisiLibity::Environment& get_vis_enviroment_offset() const;

private:
	vector2df dimension;
	std::vector<polygonf> obstacles;

	std::vector<polygonf> offset_obstacles;

	//Cached stuff
	VisiLibity::Environment vis_enviroment; //for obstacles
	VisiLibity::Environment vis_enviroment_offset; //for offset_obstacles
	VisiLibity::Visibility_Graph vis_visibility_graph_offset; //for offset_obstacles

	//TODO clean up these init_* functions
	//initializes vis_enviroment and vis_visibility_graph
	//this has to be called whenever obstacles, or dimension is modified for correct path finding
	//calls init_enviroment() and init_visilibity_graph()
	//@param : offset_delta : The distance between the original obstacles,
	//and the little bit larger polygon enclosing it, which is used for path searching
	void init_visilibity(float offset_delta);

	void init_enviroment(float offset_delta);
	void init_visilibity_graph();
};

} //namespace gc

#endif /* MAP_HPP_ */
