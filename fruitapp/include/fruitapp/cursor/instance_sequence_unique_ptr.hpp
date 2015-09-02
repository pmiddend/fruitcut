#ifndef FRUITAPP_CURSOR_INSTANCE_SEQUENCE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_CURSOR_INSTANCE_SEQUENCE_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/cursor/instance_sequence_fwd.hpp>
#include <fcppt/unique_ptr.hpp>


namespace fruitapp
{
namespace cursor
{
typedef
fcppt::unique_ptr<fruitapp::cursor::instance_sequence>
instance_sequence_unique_ptr;
}
}

#endif
