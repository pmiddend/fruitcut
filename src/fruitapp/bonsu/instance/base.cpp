#include <fruitapp/bonsu/manager.hpp>
#include <fruitapp/bonsu/instance/base.hpp>


fruitapp::bonsu::instance::base::~base()
{
}

fruitapp::bonsu::instance::base::base(
	bonsu::manager &_manager)
{
	_manager.insert(
		*this);
}
