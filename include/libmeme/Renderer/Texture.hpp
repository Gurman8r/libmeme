#ifndef _ML_TEXTURE_HPP_
#define _ML_TEXTURE_HPP_

#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Renderer/Image.hpp>
#include <libmeme/Renderer/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum TextureFlags_ : int32_t
	{
		TextureFlags_None		= 0,
		TextureFlags_Smooth		= (1 << 0),
		TextureFlags_Repeated	= (1 << 1),
		TextureFlags_Mipmapped	= (1 << 2),
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_RENDERER_API Texture final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture();
		explicit Texture(uint32_t sampler, uint32_t format, int32_t flags);
		explicit Texture(uint32_t sampler, uint32_t internalFormat, uint32_t colorFormat, int32_t flags);
		explicit Texture(uint32_t sampler, int32_t level, uint32_t internalFormat, uint32_t colorFormat, uint32_t pixelType, int32_t flags);
		explicit Texture(Path const & filename);
		Texture(Image const & image);
		Texture(Texture const & copy);
		Texture(Texture && copy) noexcept;
		~Texture();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Texture & operator=(Texture const & other);

		Texture & operator=(Texture && other) noexcept;

		void swap(Texture & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool create();

		bool destroy();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool loadFromFile(Path const & filename);
		
		bool loadFromImage(Image const & image);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static void bind(Texture const * value);

		uint32_t channels() const noexcept;

		Image to_image() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator bool() const noexcept { return m_handle; }

		inline auto handle() const noexcept -> uint32_t const & { return m_handle; }
		
		inline auto sampler() const noexcept -> uint32_t { return m_sampler; }
		
		inline auto level() const noexcept -> int32_t { return m_level; }
		
		inline auto internalFormat() const noexcept -> uint32_t { return m_internalFormat; }
		
		inline auto colorFormat() const noexcept -> uint32_t { return m_colorFormat; }
		
		inline auto pixelType() const noexcept -> uint32_t { return m_pixelType; }
		
		inline auto size() const noexcept -> vec2u const & { return m_size; }
		
		inline auto realSize() const noexcept -> vec2u const & { return m_realSize; }
		
		inline auto flags() const noexcept -> uint32_t { return m_flags; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union
		{
			uint32_t	m_handle;
			uint32_t	m_sampler;
			int32_t		m_level;
			uint32_t	m_internalFormat;
			uint32_t	m_colorFormat;
			uint32_t	m_pixelType;
			vec2u		m_size;
			vec2u		m_realSize;
			int32_t		m_flags;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static inline auto make_texture(Args && ... args)
	{
		return Texture{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TEXTURE_HPP_