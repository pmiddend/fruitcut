#ifndef FRUITAPP_CURSOR_INSTANCE_SEQUENCE_SCOPED_PTR_HPP_INCLUDED
#define FRUITAPP_CURSOR_INSTANCE_SEQUENCE_SCOPED_PTR_HPP_INCLUDED

#include <fruitapp/cursor/instance_sequence_fwd.hpp>
#include <fcppt/scoped_ptr.hpp>


namespace fruitapp
{
namespace cursor
{
typedef
fcppt::scoped_ptr<fruitapp::cursor::instance_sequence>
instance_sequence_scoped_ptr;
}
}

#endif
