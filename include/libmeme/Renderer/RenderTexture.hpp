#ifndef _ML_RENDER_TEXTURE_HPP_
#define _ML_RENDER_TEXTURE_HPP_

#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API RenderTexture final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderTexture();
		RenderTexture(RenderTexture const & other);
		RenderTexture(RenderTexture && other) noexcept;
		~RenderTexture();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderTexture & operator=(RenderTexture const & other);
		RenderTexture & operator=(RenderTexture && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void assign(RenderTexture const & other);
		void swap(RenderTexture & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_RENDER_TEXTURE_HPP_