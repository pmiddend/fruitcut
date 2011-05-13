#include "system_node.hpp"
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <sge/sprite/default_equal.hpp>

fruitcut::app::point_sprite::system_node::system_node(
	sge::renderer::device &_renderer)
:
	system_(
		_renderer)
{
}

void
fruitcut::app::point_sprite::system_node::push_back(
	unique_base_ptr n)
{
	fcppt::container::ptr::push_back_unique_ptr(
		children_,
		fcppt::move(
			n));
}

fruitcut::app::point_sprite::system &
fruitcut::app::point_sprite::system_node::system()
{
	return system_;
}

fruitcut::app::point_sprite::system const &
fruitcut::app::point_sprite::system_node::system() const
{
	return system_;
}

fruitcut::app::point_sprite::system_node::~system_node()
{
}

void
fruitcut::app::point_sprite::system_node::update()
{
	for(child_sequence::iterator i = children_.begin(); i != children_.end();)
	{
		i->update();
		if(i->dead())
			i = 
				children_.erase(
					i);
		else
			i++;
	}
}

void
fruitcut::app::point_sprite::system_node::render()
{
	system_.render_all(
		sge::sprite::default_equal());
}
