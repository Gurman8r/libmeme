#ifndef _ML_TEXTURE_HPP_
#define _ML_TEXTURE_HPP_

#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Renderer/Image.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum TextureFlags_ : int32_t
	{
		TextureFlags_None,
		TextureFlags_Smooth		= (1 << 0),
		TextureFlags_Repeated	= (1 << 1),
		TextureFlags_Mipmapped	= (1 << 2),

		// Smooth / Repeated
		TextureFlags_Default
			= TextureFlags_Smooth
			| TextureFlags_Repeated,
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API Texture final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture(uint32_t sampler, int32_t level, uint32_t internalFormat, uint32_t colorFormat, uint32_t pixelType, int32_t flags);
		Texture(uint32_t sampler, uint32_t internalFormat, uint32_t colorFormat, int32_t flags);
		Texture(uint32_t sampler, uint32_t format, int32_t flags);
		Texture(uint32_t sampler, int32_t flags);
		Texture(uint32_t sampler);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture();
		Texture(path_t const & path);
		Texture(Image const & image);
		Texture(Texture const & other);
		Texture(Texture && other) noexcept;
		~Texture();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture & operator=(Texture const & other);

		Texture & operator=(Texture && other) noexcept;

		void swap(Texture & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool load_from_file(path_t const & path);

		bool load_from_image(Image const & image);

		bool load_from_texture(Texture const & other);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool generate();

		bool destroy();

		static void bind(Texture const * value);

		inline void bind() const { bind(this); }

		inline void unbind() const { bind(nullptr); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create(vec2u const & size);
		
		bool create(Image const & image, vec2u const & size);
		
		bool create(Image const & image, uint32_t w, uint32_t h);
		
		bool create(byte_t const * pixels, vec2u const & size);
		
		bool create(byte_t const * pixels, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool update(Texture const & other, UintRect const & area);
		
		bool update(Texture const & other, vec2u const & position, vec2u const & size);
		
		bool update(Texture const & other, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool update(Image const & image);
		
		bool update(Image const & image, UintRect const & area);
		
		bool update(Image const & image, vec2u const & position, vec2u const & size);
		
		bool update(Image const & image, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool update(byte_t const * pixels);
		
		bool update(byte_t const * pixels, UintRect const & area);
		
		bool update(byte_t const * pixels, vec2u const & position, vec2u const & size);
		
		bool update(byte_t const * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool set_mipmapped(bool value);

		bool set_repeated(bool value);

		bool set_smooth(bool value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD uint32_t channels() const noexcept;

		ML_NODISCARD Image copy_to_image() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline operator bool() const noexcept { return m_handle; }

		ML_NODISCARD inline auto address() const noexcept -> void * { return ML_ADDRESSOF(m_handle); }

		ML_NODISCARD inline auto handle() const noexcept -> uint32_t const & { return m_handle; }
		
		ML_NODISCARD inline auto sampler() const noexcept -> uint32_t { return m_sampler; }
		
		ML_NODISCARD inline auto level() const noexcept -> int32_t { return m_level; }
		
		ML_NODISCARD inline auto internalFormat() const noexcept -> uint32_t { return m_internalFormat; }
		
		ML_NODISCARD inline auto colorFormat() const noexcept -> uint32_t { return m_colorFormat; }
		
		ML_NODISCARD inline auto pixelType() const noexcept -> uint32_t { return m_pixelType; }
		
		ML_NODISCARD inline auto size() const noexcept -> vec2u const & { return m_size; }
		
		ML_NODISCARD inline auto realSize() const noexcept -> vec2u const & { return m_realSize; }
		
		ML_NODISCARD inline auto flags() const noexcept -> uint32_t { return m_flags; }

		ML_NODISCARD inline auto width() const noexcept -> uint32_t { return m_size[0]; }

		ML_NODISCARD inline auto height() const noexcept -> uint32_t { return m_size[1]; }

		ML_NODISCARD inline bool smooth() const noexcept { return m_flags & TextureFlags_Smooth; }

		ML_NODISCARD inline bool repeated() const noexcept { return m_flags & TextureFlags_Repeated; }

		ML_NODISCARD inline bool mipmapped() const noexcept { return m_flags & TextureFlags_Mipmapped; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		uint32_t	m_handle;
		uint32_t	m_sampler;
		int32_t		m_level;
		uint32_t	m_internalFormat;
		uint32_t	m_colorFormat;
		uint32_t	m_pixelType;
		vec2u		m_size;
		vec2u		m_realSize;
		int32_t		m_flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_texture(Args && ... args)
	{
		return Texture{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TEXTURE_HPP_