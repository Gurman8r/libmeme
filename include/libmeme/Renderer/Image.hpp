#ifndef _ML_IMAGE_HPP_
#define _ML_IMAGE_HPP_

#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Core/Rect.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Core/FileSystem.hpp>

namespace ml
{
	struct ML_RENDERER_API Image final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Pixels					= typename std::vector<byte_t>;
		using iterator					= typename Pixels::iterator;
		using const_iterator			= typename Pixels::const_iterator;
		using reverse_iterator			= typename Pixels::reverse_iterator;
		using const_reverse_iterator	= typename Pixels::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static Image const Default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image();
		Image(vec2u const & size);
		Image(vec2u const & size, size_t channels);
		Image(vec2u const & size, Pixels const & pixels);
		Image(vec2u const & size, size_t channels, Pixels const & pixels);
		Image(path_t const & path);
		Image(path_t const & path, bool flip);
		Image(path_t const & path, bool flip, size_t req_comp);
		Image(Image const & other);
		Image(Image && other) noexcept;
		~Image();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & operator=(Image const & other);

		Image & operator=(Image && other) noexcept;

		void swap(Image & other);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool load_from_file(path_t const & path);
		
		bool load_from_file(path_t const & path, bool flip);
		
		bool load_from_file(path_t const & path, bool flip, size_t req_comp);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & createFromColor(vec2u const & size, Color32 const & color);
		
		Image & createFromColor(Color32 const & color);
		
		Image & createFromColor(vec2u const & size, size_t channels, Color32 const & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & createFromPixels(vec2u const & size, Pixels const & pixels);
		
		Image & createFromPixels(Pixels const & pixels);
		
		Image & createFromPixels(vec2u const & size, size_t channels, Pixels const & pixels);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		
		Image & flipHorizontally();

		Image & flipVertically();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		uint32_t getFormat() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		std::optional<Color32> getPixel(size_t index) const;
		
		std::optional<Color32> getPixel(size_t x, size_t y) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool setPixel(size_t index, Color32 const & color);
		
		bool setPixel(size_t x, size_t y, Color32 const & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto bounds() const noexcept -> UintRect { return UintRect { { 0, 0 }, size() }; }
		
		inline auto capacity() const noexcept -> size_t { return width() * height() * channels(); }
		
		inline auto channels() const noexcept -> size_t { return m_channels; }

		inline auto data() noexcept -> byte_t * { return m_pixels.data(); }
		
		inline auto data() const noexcept -> byte_t const * { return m_pixels.data(); }
		
		inline bool empty() const noexcept { return m_pixels.empty(); }
		
		inline auto height() const noexcept -> size_t { return m_size[1]; }

		inline auto pixels() noexcept -> Pixels & { return m_pixels; }
		
		inline auto pixels() const noexcept -> Pixels const & { return m_pixels; }
		
		inline auto size() const noexcept -> vec2u const & { return m_size; }
		
		inline auto width() const noexcept -> size_t { return m_size[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline byte_t const & operator[](size_t i) const
		{
			return m_pixels.at(i);
		}

		inline byte_t & operator[](size_t i)
		{
			return m_pixels.at(i);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin() noexcept -> iterator { return m_pixels.begin(); }
		inline auto begin() const noexcept -> const_iterator { return m_pixels.begin(); }
		inline auto cbegin() const noexcept -> const_iterator { return m_pixels.cbegin(); }

		inline auto end() noexcept -> iterator { return m_pixels.end(); }
		inline auto end() const noexcept -> const_iterator { return m_pixels.end(); }
		inline auto cend() const noexcept -> const_iterator { return m_pixels.cend(); }

		inline auto rbegin() noexcept -> reverse_iterator { return m_pixels.rbegin(); }
		inline auto rbegin() const noexcept -> const_reverse_iterator { return m_pixels.rbegin(); }
		inline auto crbegin() const noexcept -> const_reverse_iterator { return m_pixels.crbegin(); }

		inline auto rend() noexcept -> reverse_iterator { return m_pixels.rend(); }
		inline auto rend() const noexcept -> const_reverse_iterator { return m_pixels.rend(); }
		inline auto crend() const noexcept -> const_reverse_iterator { return m_pixels.crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec2u	m_size;
		Pixels	m_pixels;
		size_t	m_channels;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> static inline auto make_image(Args && ... args)
	{
		return Image{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMAGE_HPP_