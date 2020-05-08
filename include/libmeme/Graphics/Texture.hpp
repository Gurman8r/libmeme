#ifndef _ML_TEXTURE_HPP_
#define _ML_TEXTURE_HPP_

#include <libmeme/Graphics/Image.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum texture_flags_ : int32_t
	{
		texture_flags_none		= (0 << 0), // none
		texture_flags_smooth	= (1 << 0), // smooth
		texture_flags_repeated	= (1 << 1), // repeated
		texture_flags_mipmapped	= (1 << 2), // mipmapped

		// smooth / repeated
		texture_flags_default
			= texture_flags_smooth
			| texture_flags_repeated,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API texture final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		texture(uint32_t sampler, int32_t level, uint32_t internal_format, uint32_t color_format, uint32_t pixel_type, int32_t flags);
		
		texture(uint32_t sampler, uint32_t internal_format, uint32_t color_format, int32_t flags);
		
		texture(uint32_t sampler, uint32_t format, int32_t flags);
		
		texture(uint32_t sampler, int32_t flags);
		
		texture(uint32_t sampler);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		texture();
		
		texture(fs::path const & path);
		
		texture(image const & image);
		
		texture(texture const & value);
		
		texture(texture && value) noexcept;
		
		~texture() noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		texture & operator=(texture const & value)
		{
			texture temp{ value };
			swap(temp);
			return (*this);
		}

		texture & operator=(texture && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(fs::path const & path);

		bool load_from_image(image const & img);

		bool load_from_texture(texture const & value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(texture const * value);

		void bind() const { bind(this); }

		void unbind() const { bind(nullptr); }

		bool destroy();

		bool generate();

		void swap(texture & value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create(vec2u const & size);

		bool create(byte_t const * pixels, vec2u const & size);
		
		bool create(byte_t const * pixels, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool update(byte_t const * pixels, uint_rect const & area);
		
		bool update(byte_t const * pixels, vec2u const & pos, vec2u const & size);
		
		bool update(byte_t const * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD uint32_t channels() const noexcept;

		ML_NODISCARD image copy_to_image() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool set_mipmapped(bool value);

		bool set_repeated(bool value);

		bool set_smooth(bool value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool nonzero() const noexcept { return m_handle; }

		ML_NODISCARD operator bool() const noexcept { return this->nonzero(); }

		ML_NODISCARD auto address() const noexcept -> void * { return ML_addressof(m_handle); }

		ML_NODISCARD auto handle() const noexcept -> uint32_t const & { return m_handle; }
		
		ML_NODISCARD auto sampler() const noexcept -> uint32_t { return m_sampler; }
		
		ML_NODISCARD auto level() const noexcept -> int32_t { return m_level; }
		
		ML_NODISCARD auto internal_format() const noexcept -> uint32_t { return m_intl_format; }
		
		ML_NODISCARD auto color_format() const noexcept -> uint32_t { return m_color_format; }
		
		ML_NODISCARD auto pixel_type() const noexcept -> uint32_t { return m_pixel_type; }
		
		ML_NODISCARD auto size() const noexcept -> vec2u const & { return m_size; }
		
		ML_NODISCARD auto real_size() const noexcept -> vec2u const & { return m_real_size; }
		
		ML_NODISCARD auto flags() const noexcept -> uint32_t { return m_flags; }

		ML_NODISCARD auto width() const noexcept -> uint32_t { return m_size[0]; }

		ML_NODISCARD auto height() const noexcept -> uint32_t { return m_size[1]; }

		ML_NODISCARD bool is_smooth() const noexcept { return m_flags & texture_flags_smooth; }

		ML_NODISCARD bool is_repeated() const noexcept { return m_flags & texture_flags_repeated; }

		ML_NODISCARD bool is_mipmapped() const noexcept { return m_flags & texture_flags_mipmapped; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint32_t	m_handle;
		uint32_t	m_sampler;
		int32_t		m_level;
		uint32_t	m_intl_format;
		uint32_t	m_color_format;
		uint32_t	m_pixel_type;
		vec2u		m_size;
		vec2u		m_real_size;
		int32_t		m_flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD inline auto make_texture(Args && ... args)
	{
		return texture{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TEXTURE_HPP_