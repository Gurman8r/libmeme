#ifndef _ML_IMAGE_HPP_
#define _ML_IMAGE_HPP_

#include <libmeme/Renderer/Color.hpp>
#include <libmeme/Core/Rect.hpp>
#include <libmeme/Core/MemoryTracker.hpp>
#include <libmeme/Platform/FileSystem.hpp>

namespace ml
{
	struct ML_RENDERER_API Image final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type			= typename pmr::polymorphic_allocator<byte_t>;
		using pixels_type				= typename pmr::vector<byte_t>;
		using iterator					= typename pixels_type::iterator;
		using const_iterator			= typename pixels_type::const_iterator;
		using reverse_iterator			= typename pixels_type::reverse_iterator;
		using const_reverse_iterator	= typename pixels_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static Image const Default;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image();
		
		explicit Image(allocator_type const & alloc);
		
		Image(vec2u const & size, allocator_type const & alloc = {});
		
		Image(vec2u const & size, size_t channels, allocator_type const & alloc = {});
		
		Image(vec2u const & size, pixels_type const & pixels, allocator_type const & alloc = {});
		
		Image(vec2u const & size, size_t channels, pixels_type const & pixels, allocator_type const & alloc = {});
		
		Image(path_t const & path, allocator_type const & alloc = {});
		
		Image(path_t const & path, bool flip, allocator_type const & alloc = {});
		
		Image(path_t const & path, bool flip, size_t req_comp, allocator_type const & alloc = {});
		
		Image(Image const & other, allocator_type const & alloc = {});
		
		Image(Image && other, allocator_type const & alloc = {}) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & operator=(Image const & other);

		Image & operator=(Image && other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool load_from_file(path_t const & path);
		
		bool load_from_file(path_t const & path, bool flip);
		
		bool load_from_file(path_t const & path, bool flip, size_t req_comp);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & create_from_color(vec2u const & size, Color32 const & color);
		
		Image & create_from_color(Color32 const & color);
		
		Image & create_from_color(vec2u const & size, size_t channels, Color32 const & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Image & create_from_pixels(vec2u const & size, pixels_type const & pixels);
		
		Image & create_from_pixels(pixels_type const & pixels);
		
		Image & create_from_pixels(vec2u const & size, size_t channels, pixels_type const & pixels);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept;

		void swap(Image & other) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		Image & flip_vertically();

		Image & flip_horizontally();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD uint32_t get_format() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD std::optional<Color32> get_pixel(size_t index) const;
		
		ML_NODISCARD std::optional<Color32> get_pixel(size_t x, size_t y) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool set_pixel(size_t index, Color32 const & color);
		
		bool set_pixel(size_t x, size_t y, Color32 const & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto operator[](size_t i) -> byte_t & { return m_pixels.at(i); }

		ML_NODISCARD inline auto operator[](size_t i) const -> byte_t const & { return m_pixels.at(i); }

		ML_NODISCARD inline auto bounds() const noexcept -> uint_rect { return uint_rect { { 0, 0 }, size() }; }
		
		ML_NODISCARD inline auto capacity() const noexcept -> size_t { return width() * height() * channels(); }
		
		ML_NODISCARD inline auto channels() const noexcept -> size_t { return m_channels; }

		ML_NODISCARD inline auto data() noexcept -> byte_t * { return m_pixels.data(); }
		
		ML_NODISCARD inline auto data() const noexcept -> byte_t const * { return m_pixels.data(); }
		
		ML_NODISCARD inline bool empty() const noexcept { return m_pixels.empty(); }
		
		ML_NODISCARD inline auto height() const noexcept -> size_t { return m_size[1]; }

		ML_NODISCARD inline auto pixels() noexcept -> pixels_type & { return m_pixels; }
		
		ML_NODISCARD inline auto pixels() const noexcept -> pixels_type const & { return m_pixels; }
		
		ML_NODISCARD inline auto size() const noexcept -> vec2u const & { return m_size; }
		
		ML_NODISCARD inline auto width() const noexcept -> size_t { return m_size[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD inline auto begin() noexcept -> iterator { return m_pixels.begin(); }
		
		ML_NODISCARD inline auto begin() const noexcept -> const_iterator { return m_pixels.begin(); }
		
		ML_NODISCARD inline auto cbegin() const noexcept -> const_iterator { return m_pixels.cbegin(); }

		ML_NODISCARD inline auto cend() const noexcept -> const_iterator { return m_pixels.cend(); }

		ML_NODISCARD inline auto crbegin() const noexcept -> const_reverse_iterator { return m_pixels.crbegin(); }

		ML_NODISCARD inline auto crend() const noexcept -> const_reverse_iterator { return m_pixels.crend(); }

		ML_NODISCARD inline auto end() noexcept -> iterator { return m_pixels.end(); }
		
		ML_NODISCARD inline auto end() const noexcept -> const_iterator { return m_pixels.end(); }

		ML_NODISCARD inline auto rbegin() noexcept -> reverse_iterator { return m_pixels.rbegin(); }
		
		ML_NODISCARD inline auto rbegin() const noexcept -> const_reverse_iterator { return m_pixels.rbegin(); }

		ML_NODISCARD inline auto rend() noexcept -> reverse_iterator { return m_pixels.rend(); }
		
		ML_NODISCARD inline auto rend() const noexcept -> const_reverse_iterator { return m_pixels.rend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec2u		m_size;
		size_t		m_channels;
		pixels_type	m_pixels;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static inline auto make_image(Args && ... args)
	{
		return Image{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMAGE_HPP_