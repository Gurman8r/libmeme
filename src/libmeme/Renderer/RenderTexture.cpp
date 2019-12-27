#include <libmeme/Renderer/RenderTexture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTexture::RenderTexture()
	{
	}

	RenderTexture::RenderTexture(RenderTexture const & other)
	{
	}

	RenderTexture::RenderTexture(RenderTexture && other) noexcept
	{
	}

	RenderTexture::~RenderTexture()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RenderTexture & RenderTexture::operator=(RenderTexture const & other)
	{
		assign(other);
		return (*this);
	}

	RenderTexture & RenderTexture::operator=(RenderTexture && other) noexcept
	{
		swap(std::move(other));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void RenderTexture::assign(RenderTexture const & other)
	{
		if (this != std::addressof(other))
		{

		}
	}

	void RenderTexture::swap(RenderTexture & other) noexcept
	{
		if (this != std::addressof(other))
		{

		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}