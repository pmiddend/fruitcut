#ifndef FRUITAPP_FRUIT_PROTOTYPE_FROM_JSON_HPP_INCLUDED
#define FRUITAPP_FRUIT_PROTOTYPE_FROM_JSON_HPP_INCLUDED

#include "prototype.hpp"
#include <sge/parse/json/value.hpp>
#include <sge/model/md3/loader_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>

namespace fruitapp
{
namespace fruit
{
fruit::prototype const
prototype_from_json(
	sge::parse::json::value const &,
	sge::model::md3::loader &,
	sge::image2d::multi_loader &,
	sge::renderer::device &);
}
}

#endif
