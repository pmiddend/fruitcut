#include "group.hpp"

fruitcut::scene::nodes::group::child_sequence const &
fruitcut::scene::nodes::group::children() const
{
	return children_;
}

fruitcut::scene::nodes::group::child_sequence &
fruitcut::scene::nodes::group::children()
{
	return children_;
}

void
fruitcut::scene::nodes::group::update()
{
	for(child_sequence::iterator i = children_.begin(); i != children_.end(); ++i)
		i->update();
}

void
fruitcut::scene::nodes::group::render()
{
	for(child_sequence::iterator i = children_.begin(); i != children_.end(); ++i)
		i->render();
}

fruitcut::scene::nodes::group::group()
:	
	children_()
{
}
