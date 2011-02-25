#include "declaration_buffer_pair.hpp"

fruitcut::pp::screen_vf::declaration_buffer_pair::declaration_buffer_pair()
:
	declaration_(),
	buffer_()
{
}

fruitcut::pp::screen_vf::declaration_buffer_pair::declaration_buffer_pair(
	sge::renderer::vertex_declaration_ptr const _declaration,
	sge::renderer::vertex_buffer_ptr const _buffer)
:
	declaration_(
		_declaration),
	buffer_(
		_buffer)
{
}

sge::renderer::vertex_declaration_ptr const 
fruitcut::pp::screen_vf::declaration_buffer_pair::declaration() const
{
	return declaration_;
}

sge::renderer::vertex_buffer_ptr const 
fruitcut::pp::screen_vf::declaration_buffer_pair::buffer() const
{
	return buffer_;
}

fruitcut::pp::screen_vf::declaration_buffer_pair::~declaration_buffer_pair()
{
}
