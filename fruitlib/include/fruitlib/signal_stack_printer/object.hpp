#ifndef FRUITLIB_SIGNAL_STACK_PRINTER_OBJECT_HPP_INCLUDED
#define FRUITLIB_SIGNAL_STACK_PRINTER_OBJECT_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/scoped_ptr.hpp>

namespace fruitlib
{
namespace signal_stack_printer
{
class object_impl;

class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	object();

	~object();
private:
	fcppt::scoped_ptr<signal_stack_printer::object_impl> impl_;
};
}
}

#endif

