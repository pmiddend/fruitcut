#include "group.hpp"
#include "with_lifetime.hpp"
#include <fcppt/try_dynamic_cast.hpp>

fruitcut::scenic::nodes::group::child_sequence const &
fruitcut::scenic::nodes::group::children() const
{
	return children_;
}

fruitcut::scenic::nodes::group::child_sequence &
fruitcut::scenic::nodes::group::children()
{
	return children_;
}

void
fruitcut::scenic::nodes::group::update()
{
	for(child_sequence::iterator i = children_.begin(); i != children_.end();)
	{
		i->update();

		FCPPT_TRY_DYNAMIC_CAST(
			with_lifetime*,
			has_lifetime,
			&(*i))
		{
			if(has_lifetime->dead())
				i = children_.erase(i);
			else
				++i;
		}
		else
		{
			++i;
		}
	}
}

void
fruitcut::scenic::nodes::group::render()
{
	for(child_sequence::iterator i = children_.begin(); i != children_.end(); ++i)
		i->render();
}

fruitcut::scenic::nodes::group::group()
:	
	children_()
{
}
