#include <fruitapp/bonsu/instance/base.hpp>
#include <fruitapp/bonsu/manager.hpp>

fruitapp::bonsu::instance::base::~base()
{
}

fruitapp::bonsu::instance::base::base(
	bonsu::manager &_manager)
{
	_manager.insert(
		*this);
}
