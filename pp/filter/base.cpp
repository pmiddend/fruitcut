#include "base.hpp"
#include "../texture/counted_instance.hpp"
#include "../texture/instance.hpp"
#include "nullary.hpp"
#include "unary.hpp"
#include "binary.hpp"
#include <fcppt/assert_message.hpp>
#include <fcppt/text.hpp>

fruitcut::pp::filter::base::base()
{
}

fruitcut::pp::filter::base::~base()
{
}

// for an explanation see system.hpp
void
fruitcut::pp::filter::base::enqueue(
	texture::counted_instance const &tex)
{
	textures_.push_back(
		tex);
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::filter::base::dispatch()
{
	texture::counted_instance result;
	if (textures_.empty())
		result = 
			dynamic_cast<nullary &>(*this).apply();
	else if (textures_.size() == 1)
		result = 
			dynamic_cast<unary &>(*this).apply(
				textures_.front());
	else if (textures_.size() == 2)
		result = 
			dynamic_cast<binary &>(*this).apply(
				textures_.front(),
				textures_.back());
	textures_.clear();
	FCPPT_ASSERT_MESSAGE(
		result,
		FCPPT_TEXT("Got an empty texture as a filter result"));
	return result;
}
