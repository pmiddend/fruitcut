#ifndef FRUITCUT_PP_SCREEN_VF_DECLARATION_BUFFER_PAIR_HPP_INCLUDED
#define FRUITCUT_PP_SCREEN_VF_DECLARATION_BUFFER_PAIR_HPP_INCLUDED

#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>

namespace fruitcut
{
namespace pp
{
namespace screen_vf
{
class declaration_buffer_pair
{
public:
	// In filter::blur we want to store the pair in an array.
	explicit
	declaration_buffer_pair();

	explicit
	declaration_buffer_pair(
		sge::renderer::vertex_declaration_ptr,
		sge::renderer::vertex_buffer_ptr);

	sge::renderer::vertex_declaration_ptr const 
	declaration() const;

	sge::renderer::vertex_buffer_ptr const 
	buffer() const;

	~declaration_buffer_pair();
private:
	sge::renderer::vertex_declaration_ptr declaration_;
	sge::renderer::vertex_buffer_ptr buffer_;
};
}
}
}

#endif
