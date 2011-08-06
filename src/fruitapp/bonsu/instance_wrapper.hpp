#ifndef FRUITAPP_BONSU_INSTANCE_WRAPPER_HPP_INCLUDED
#define FRUITAPP_BONSU_INSTANCE_WRAPPER_HPP_INCLUDED

#include <fruitapp/bonsu/instance/base_fwd.hpp>
#include <fruitapp/bonsu/rectangle/instance.hpp>
#include <fruitapp/bonsu/sprite/system.hpp>
#include <fruitapp/bonsu/sprite/object.hpp>
#include <fruitapp/bonsu/rectangle/manager.hpp>
#include <fruitapp/bonsu/texture_manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/scoped_ptr.hpp>

namespace fruitapp
{
namespace bonsu
{
class instance_wrapper
{
FCPPT_NONCOPYABLE(
	instance_wrapper);
public:
	explicit
	instance_wrapper(
		bonsu::instance::base &,
		bonsu::sprite::system &,
		bonsu::texture_manager &,
		bonsu::rectangle::manager &);

	void
	update();

	bonsu::instance::base &
	instance() const;

	~instance_wrapper();
private:
	bonsu::instance::base &instance_;
	bonsu::sprite::object main_sprite_;
	bonsu::sprite::object overlay_sprite_;
	bonsu::rectangle::manager &rectangle_manager_;
	fcppt::scoped_ptr<bonsu::rectangle::instance> rectangle_instance_;
	bool previous_active_state_;

	void
	synchronize();
};
}
}

#endif
