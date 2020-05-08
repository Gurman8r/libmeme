#ifndef _ML_RENDER_TEXTURE_HPP_
#define _ML_RENDER_TEXTURE_HPP_

#include <libmeme/Graphics/Material.hpp>
#include <libmeme/Graphics/Model.hpp>
#include <libmeme/Graphics/RenderTarget.hpp>

namespace ml
{
	struct ML_GRAPHICS_API render_texture final : trackable, render_target
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_texture();

		render_texture(vec2i const & size);

		render_texture(vec2i const & size, uint32_t colorID, uint32_t frameID, uint32_t format);

		render_texture(render_texture const & value);

		render_texture(render_texture && value) noexcept;

		~render_texture() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		render_texture & operator=(render_texture const & value)
		{
			render_texture temp{ value };
			swap(temp);
			return (*this);
		}

		render_texture & operator=(render_texture && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		void swap(render_texture & value) noexcept
		{
			if (this != std::addressof(value))
			{
				std::swap(m_fbo, value.m_fbo);
				std::swap(m_rbo, value.m_rbo);
				std::swap(m_size, value.m_size);
				std::swap(m_colorID, value.m_colorID);
				std::swap(m_frameID, value.m_frameID);
				std::swap(m_format, value.m_format);
				std::swap(m_texture, value.m_texture);
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

		ML_NODISCARD bool nonzero() const noexcept { return m_fbo && m_rbo && m_texture; }

		ML_NODISCARD operator bool() const noexcept { return this->nonzero(); }

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