#ifndef _ML_RENDER_TEXTURE_HPP_
#define _ML_RENDER_TEXTURE_HPP_

#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Model.hpp>
#include <libmeme/Renderer/Shader.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>
#include <libmeme/Renderer/Texture.hpp>

namespace ml
{
	struct ML_RENDERER_API render_texture final : trackable, render_target
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_texture();

		render_texture(vec2i const & size);

		render_texture(vec2i const & size, uint32_t colorID, uint32_t frameID, uint32_t format);

		render_texture(render_texture const & other);

		render_texture(render_texture && other) noexcept;

		~render_texture();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_texture & operator=(render_texture const & other);

		render_texture & operator=(render_texture && other) noexcept;

		void swap(render_texture & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create();

		bool destroy();

		bool resize(vec2i const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(render_texture const * value);

		inline void bind() const { return bind(this); }

		inline void unbind() const { return bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept { return good(); }

		ML_NODISCARD inline bool good() const noexcept { return m_fbo && m_rbo && m_texture; }

		ML_NODISCARD inline auto fbo() const noexcept -> FBO const & { return m_fbo; }

		ML_NODISCARD inline auto rbo() const noexcept -> RBO const & { return m_rbo; }

		ML_NODISCARD inline auto size() const noexcept -> vec2i const & { return m_size; }

		ML_NODISCARD inline auto width() const noexcept -> int32_t { return m_size[0]; }

		ML_NODISCARD inline auto height() const noexcept -> int32_t { return m_size[1]; }

		ML_NODISCARD inline auto bounds() const noexcept -> int_rect { return int_rect{ 0, 0, width(), height() }; }

		ML_NODISCARD inline auto colorID() const noexcept -> uint32_t { return m_colorID; }

		ML_NODISCARD inline auto frameID() const noexcept -> uint32_t { return m_frameID; }

		ML_NODISCARD inline auto format() const noexcept -> uint32_t { return m_format; }

		ML_NODISCARD inline auto get_texture() const noexcept -> texture const & { return m_texture; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		FBO			m_fbo;
		RBO			m_rbo;
		vec2i		m_size;
		uint32_t	m_colorID;
		uint32_t	m_frameID;
		uint32_t	m_format;
		texture		m_texture;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_render_texture(Args && ... args)
	{
		return render_texture{ ML_fwd(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TEXTURE_HPP_