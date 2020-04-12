#ifndef _ML_RENDER_TEXTURE_HPP_
#define _ML_RENDER_TEXTURE_HPP_

#include <libmeme/Renderer/Material.hpp>
#include <libmeme/Renderer/Model.hpp>
#include <libmeme/Renderer/RenderTarget.hpp>

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

		~render_texture() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_texture & operator=(render_texture const & other)
		{
			render_texture temp{ other };
			swap(temp);
			return (*this);
		}

		render_texture & operator=(render_texture && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		void swap(render_texture & other) noexcept
		{
			if (this != std::addressof(other))
			{
				std::swap(m_fbo, other.m_fbo);
				std::swap(m_rbo, other.m_rbo);
				std::swap(m_size, other.m_size);
				std::swap(m_colorID, other.m_colorID);
				std::swap(m_frameID, other.m_frameID);
				std::swap(m_format, other.m_format);
				std::swap(m_texture, other.m_texture);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(render_texture const * value);

		void bind() const { bind(this); }

		void unbind() const { bind(nullptr); }

		bool generate();

		bool destroy();

		bool resize(vec2i const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return good(); }

		ML_NODISCARD bool good() const noexcept { return m_fbo && m_rbo && m_texture; }

		ML_NODISCARD auto fbo() const noexcept -> FBO const & { return m_fbo; }

		ML_NODISCARD auto rbo() const noexcept -> RBO const & { return m_rbo; }

		ML_NODISCARD auto size() const noexcept -> vec2i const & { return m_size; }

		ML_NODISCARD auto width() const noexcept -> int32_t { return m_size[0]; }

		ML_NODISCARD auto height() const noexcept -> int32_t { return m_size[1]; }

		ML_NODISCARD auto bounds() const noexcept -> int_rect { return int_rect{ 0, 0, width(), height() }; }

		ML_NODISCARD auto colorID() const noexcept -> uint32_t { return m_colorID; }

		ML_NODISCARD auto frameID() const noexcept -> uint32_t { return m_frameID; }

		ML_NODISCARD auto format() const noexcept -> uint32_t { return m_format; }

		ML_NODISCARD auto get_texture() const noexcept -> texture const & { return m_texture; }

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
	> ML_NODISCARD inline auto make_render_texture(Args && ... args)
	{
		return render_texture{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_TEXTURE_HPP_