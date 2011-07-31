#ifndef FRUITAPP_HIGHSCORE_PROVIDER_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_SEQUENCE_HPP_INCLUDED

#include "provider/object_base_fwd.hpp"
#include <boost/ptr_container/ptr_vector.hpp>

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
