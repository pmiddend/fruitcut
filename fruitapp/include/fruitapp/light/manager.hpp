#ifndef FRUITAPP_LIGHT_MANAGER_HPP_INCLUDED
#define FRUITAPP_LIGHT_MANAGER_HPP_INCLUDED

#include <fruitapp/light/ambient_intensity.hpp>
#include <fruitapp/light/directional_light_source_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace light
{
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	explicit
	manager(
		sge::parse::json::object const &);

	fruitapp::light::directional_light_source const &
	directional_source() const;

	fruitapp::light::ambient_intensity const &
	ambient_intensity() const;

	~manager();
private:
	std::unique_ptr<fruitapp::light::directional_light_source> const directional_source_;
	fruitapp::light::ambient_intensity const ambient_intensity_;
};
}
}

#endif
