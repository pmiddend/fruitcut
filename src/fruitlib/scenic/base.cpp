#include <fruitlib/scenic/base.hpp>
#include <iostream>

fruitlib::scenic::base::child_sequence const &
fruitlib::scenic::base::children() const
{
	return
		children_;
}

fruitlib::scenic::base::child_sequence &
fruitlib::scenic::base::children()
{
	return
		children_;
}

void
fruitlib::scenic::base::forward_to_children(
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

fruitlib::scenic::depth::value_type
fruitlib::scenic::base::depth() const
{
	return depth_;
}

fruitlib::scenic::base::~base()
{
}

fruitlib::scenic::base::base(
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
fruitlib::scenic::base::add_child(
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
