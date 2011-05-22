#include "intrusive_group.hpp"
#include "../exception.hpp"
#include <fcppt/text.hpp>
#include <boost/next_prior.hpp>

fruitcut::scenic::nodes::intrusive_group::intrusive_group()
:	
	children_()
{
}

fruitcut::scenic::nodes::intrusive_group::child_sequence const &
fruitcut::scenic::nodes::intrusive_group::children() const
{
	return children_;
}

fruitcut::scenic::nodes::intrusive_group::child_sequence &
fruitcut::scenic::nodes::intrusive_group::children()
{
	return children_;
}

void
fruitcut::scenic::nodes::intrusive_group::push_front(
	nodes::intrusive &new_)
{
	children_.push_front(
		new_);
}

void
fruitcut::scenic::nodes::intrusive_group::insert_dont_care(
	nodes::intrusive &new_)
{
	children_.push_back(
		new_);
}

void
fruitcut::scenic::nodes::intrusive_group::push_back(
	nodes::intrusive &new_)
{
	children_.push_back(
		new_);
}

void
fruitcut::scenic::nodes::intrusive_group::insert_before(
	nodes::intrusive &new_,
	nodes::intrusive &before)
{
	for(child_sequence::iterator it = children_.begin(); it != children_.end(); ++it)
	{
		if(&(*it) != &before)
			continue;

		children_.insert(
			it,
			new_);
		return;
	}

	throw scenic::exception(FCPPT_TEXT("insert_before failed, couldn't find before node"));
}

void
fruitcut::scenic::nodes::intrusive_group::insert_after(
	nodes::intrusive &new_,
	nodes::intrusive &before)
{
	for(child_sequence::iterator it = children_.begin(); it != children_.end(); ++it)
	{
		if(&(*it) != &before)
			continue;

		children_.insert(
			boost::next(
				it),
			new_);
		return;
	}

	throw scenic::exception(FCPPT_TEXT("insert_before failed, couldn't find before node"));
}

fruitcut::scenic::nodes::intrusive_group::~intrusive_group()
{
}

void
fruitcut::scenic::nodes::intrusive_group::update()
{
	for(child_sequence::iterator i = children_.begin(); i != children_.end(); ++i)
		i->update();
}

void
fruitcut::scenic::nodes::intrusive_group::render()
{
	for(child_sequence::iterator i = children_.begin(); i != children_.end(); ++i)
		i->render();
}
