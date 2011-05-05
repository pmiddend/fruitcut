#include "intrusive_group.hpp"

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
