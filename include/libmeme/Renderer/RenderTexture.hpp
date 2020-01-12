#ifndef _ML_RENDER_TEXTURE_HPP_
#define _ML_RENDER_TEXTURE_HPP_

#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Model.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API RenderTexture final : public Trackable, public RenderTarget
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderTexture();
		RenderTexture(vec2i const & size);
		RenderTexture(vec2i const & size, uint32_t colorID, uint32_t frameID, uint32_t format);
		RenderTexture(RenderTexture const & other);
		RenderTexture(RenderTexture && other) noexcept;
		~RenderTexture();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderTexture & operator=(RenderTexture const & other);

		RenderTexture & operator=(RenderTexture && other) noexcept;

		void swap(RenderTexture & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create();

		bool destroy();

		bool resize(vec2i const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(RenderTexture const * value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const noexcept { return fbo() && rbo(); }

		inline auto fbo() const noexcept -> FrameBufferObject const & { return m_fbo; }

		inline auto rbo() const noexcept -> RenderBufferObject const & { return m_rbo; }

		inline auto size() const noexcept -> vec2i const & { return m_size; }

		inline auto colorID() const noexcept -> uint32_t { return m_colorID; }

		inline auto frameID() const noexcept -> uint32_t { return m_frameID; }

		inline auto format() const noexcept -> uint32_t { return m_format; }

		inline auto texture() const noexcept -> Texture const & { return m_texture; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		FrameBufferObject	m_fbo;
		RenderBufferObject	m_rbo;
		vec2i				m_size;
		uint32_t			m_colorID;
		uint32_t			m_frameID;
		uint32_t			m_format;
		Texture				m_texture;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static inline auto make_render_texture(Args && ... args)
	{
		return RenderTexture{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TEXTURE_HPP_