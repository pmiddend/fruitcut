#include "base.hpp"
#include <iostream>

fruitcut::fruitlib::scenic::base::child_sequence const &
fruitcut::fruitlib::scenic::base::children() const
{
	return 
		children_;
}

fruitcut::fruitlib::scenic::base::child_sequence &
fruitcut::fruitlib::scenic::base::children()
{
	return
		children_;
}

void
fruitcut::fruitlib::scenic::base::forward_to_children(
	events::base const &e)
{
	for(
		child_sequence::iterator it = 
			children_.begin(); 
		it != children_.end(); 
		++it)
		it->process(
			e);
}

fruitcut::fruitlib::scenic::depth::value_type
fruitcut::fruitlib::scenic::base::depth() const
{
	return depth_;
}

fruitcut::fruitlib::scenic::base::~base()
{
}

fruitcut::fruitlib::scenic::base::base(
	scenic::optional_parent const &_parent)
:
	children_(),
	depth_(
		_parent
		?
			_parent->depth()
		:
			0)
{
	if(_parent)
		_parent->object().add_child(
			*this);
}

void
fruitcut::fruitlib::scenic::base::add_child(
	base &_child)
{
	for(
		child_sequence::iterator it =
			children_.begin();
		it != children_.end();
		++it)
	{
		if(it->depth() > _child.depth())
		{
			children_.insert(
				it,
				_child);
			return;
		}
	}

	children_.push_back(
		_child);
}
