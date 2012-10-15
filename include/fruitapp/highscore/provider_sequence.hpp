#ifndef FRUITAPP_HIGHSCORE_PROVIDER_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_SEQUENCE_HPP_INCLUDED

#include <fruitapp/highscore/provider/object_base_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace highscore
{
typedef
boost::ptr_vector<provider::object_base>
provider_sequence;
}
}

#endif