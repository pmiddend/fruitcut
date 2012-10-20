#include <fruitlib/pp/filter/base.hpp>
#include <fruitlib/pp/filter/binary.hpp>
#include <fruitlib/pp/filter/nullary.hpp>
#include <fruitlib/pp/filter/unary.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert/error_message.hpp>


fruitlib::pp::filter::base::base()
:
	textures_()
{
}

fruitlib::pp::filter::base::~base()
{
}

// for an explanation see system.hpp
void
fruitlib::pp::filter::base::enqueue(
	fruitlib::pp::texture::counted_instance const &tex)
{
	textures_.push_back(
		tex);
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::filter::base::dispatch()
{
	fruitlib::pp::texture::counted_instance result;
	if (textures_.empty())
		result =
			dynamic_cast<fruitlib::pp::filter::nullary &>(*this).apply();
	else if (textures_.size() == 1)
		result =
			dynamic_cast<fruitlib::pp::filter::unary &>(*this).apply(
				textures_.front());
	else if (textures_.size() == 2)
		result =
			dynamic_cast<fruitlib::pp::filter::binary &>(*this).apply(
				textures_.front(),
				textures_.back());
	textures_.clear();
	FCPPT_ASSERT_ERROR_MESSAGE(
		result,
		FCPPT_TEXT("Got an empty texture as a filter result"));
	return result;
}
