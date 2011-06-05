#ifndef FRUITCUT_APP_SCOPED_SHADOW_MAP_HPP_INCLUDED
#define FRUITCUT_APP_SCOPED_SHADOW_MAP_HPP_INCLUDED

#include "background_fwd.hpp"
#include "shadow_map_fwd.hpp"
#include  <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
class scoped_shadow_map
{
FCPPT_NONCOPYABLE(
	scoped_shadow_map);
public:
	explicit
	scoped_shadow_map(
		app::shadow_map &,
		app::background &);

	~scoped_shadow_map();
private:
	app::background &background_;
};
}
}

#endif
