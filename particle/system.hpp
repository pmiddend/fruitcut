#ifndef FRUITCUT_PARTICLE_SYSTEM_HPP_INCLUDED
#define FRUITCUT_PARTICLE_SYSTEM_HPP_INCLUDED

#include "../sprite/system.hpp"
#include "objects/unique_base_ptr.hpp"
#include "objects/base.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <boost/ptr_container/ptr_list.hpp>

namespace fruitcut
{
namespace particle
{
class system
{
public:
	explicit
	system(
		sge::renderer::device_ptr);

	void
	update();

	void 
	render();

	sprite::system &
	sprite_system();

	void
	insert(
		objects::unique_base_ptr);

	~system();
private:
	typedef
	boost::ptr_list<objects::base>
	particle_sequence;

	sprite::system ss_;
	particle_sequence particles_;
};
}
}

#endif
