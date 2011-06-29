#ifndef FRUITCUT_APP_FRUIT_PROTOTYPE_FROM_JSON_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_PROTOTYPE_FROM_JSON_HPP_INCLUDED

#include "prototype.hpp"
#include <sge/parse/json/value.hpp>
#include <sge/md3/loader_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
fruit::prototype const
prototype_from_json(
	sge::parse::json::value const &,
	sge::md3::loader &,
	sge::image2d::multi_loader &,
	sge::renderer::device &);
}
}
}

#endif
