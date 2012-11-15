#ifndef FRUITAPP_FONT_PARTICLE_LIFETIME_HPP_INCLUDED
#define FRUITAPP_FONT_PARTICLE_LIFETIME_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <boost/chrono.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace font_particle
{
typedef
boost::chrono::duration<float>
lifetime;
}
}

#endif
