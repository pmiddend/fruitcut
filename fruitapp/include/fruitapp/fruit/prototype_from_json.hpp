#ifndef FRUITAPP_FRUIT_PROTOTYPE_FROM_JSON_HPP_INCLUDED
#define FRUITAPP_FRUIT_PROTOTYPE_FROM_JSON_HPP_INCLUDED

#include <fruitapp/fruit/prototype_unique_ptr.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/model/md3/loader_fwd.hpp>
#include <sge/parse/json/value.hpp>
#include <fruitlib/texture_manager_fwd.hpp>


namespace fruitapp
{
namespace fruit
{
fruitapp::fruit::prototype_unique_ptr
prototype_from_json(
	sge::parse::json::value const &,
	sge::model::md3::loader &,
	fruitlib::texture_manager &);
}
}

#endif
