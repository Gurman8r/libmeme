#ifndef _ML_IMAGE_HPP_
#define _ML_IMAGE_HPP_

#include <libmeme/Graphics/Export.hpp>
#include <libmeme/Core/Color.hpp>
#include <libmeme/Core/Rect.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_GRAPHICS_API image final : trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using allocator_type			= typename pmr::polymorphic_allocator<byte_t>;
		using storage_type				= typename pmr::vector<byte_t>;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static image const default_rgba;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image(allocator_type const & alloc = {});
		
		image(vec2u const & size, allocator_type const & alloc = {});
		
		image(vec2u const & size, size_t channels, allocator_type const & alloc = {});
		
		image(vec2u const & size, storage_type const & pixels, allocator_type const & alloc = {});
		
		image(vec2u const & size, size_t channels, storage_type const & pixels, allocator_type const & alloc = {});
		
		image(fs::path const & path, allocator_type const & alloc = {});
		
		image(fs::path const & path, bool flip_v, allocator_type const & alloc = {});
		
		image(fs::path const & path, bool flip_v, size_t req_channels, allocator_type const & alloc = {});
		
		image(image const & value, allocator_type const & alloc = {});
		
		image(image && value, allocator_type const & alloc = {}) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image & operator=(image const & value);

		image & operator=(image && value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		bool load_from_file(fs::path const & path);
		
		bool load_from_file(fs::path const & path, bool flip_v);
		
		bool load_from_file(fs::path const & path, bool flip_v, size_t req_channels);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image & create_from_color(vec2u const & size, color32 const & color);
		
		image & create_from_color(color32 const & color);
		
		image & create_from_color(vec2u const & size, size_t channels, color32 const & color);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image & create_from_pixels(vec2u const & size, storage_type const & pixels);
		
		image & create_from_pixels(storage_type const & pixels);
		
		image & create_from_pixels(vec2u const & size, size_t channels, storage_type const & pixels);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept;

		void swap(image & value) noexcept;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		image & flip_vertically();

		image & flip_horizontally();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD std::optional<color32> get_pixel(size_t index) const;
		
		ML_NODISCARD std::optional<color32> get_pixel(size_t x, size_t y) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool set_pixel(size_t index, color32 const & col);
		
		bool set_pixel(size_t x, size_t y, color32 const & col);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD bool good() const noexcept { return !m_pixels.empty(); }

		ML_NODISCARD operator bool() const noexcept { return good(); }

		ML_NODISCARD auto operator[](size_t i) -> byte_t & { return m_pixels.at(i); }

		ML_NODISCARD auto operator[](size_t i) const -> byte_t const & { return m_pixels.at(i); }

		ML_NODISCARD auto bounds() const noexcept -> uint_rect { return uint_rect { {}, size() }; }
		
		ML_NODISCARD auto capacity() const noexcept -> size_t { return width() * height() * channels(); }
		
		ML_NODISCARD auto channels() const noexcept -> size_t { return m_channels; }

		ML_NODISCARD auto data() noexcept -> byte_t * { return m_pixels.data(); }
		
		ML_NODISCARD auto data() const noexcept -> byte_t const * { return m_pixels.data(); }
		
		ML_NODISCARD bool empty() const noexcept { return m_pixels.empty(); }
		
		ML_NODISCARD auto height() const noexcept -> size_t { return m_size[1]; }

		ML_NODISCARD auto pixels() noexcept -> storage_type & { return m_pixels; }
		
		ML_NODISCARD auto pixels() const noexcept -> storage_type const & { return m_pixels; }
		
		ML_NODISCARD auto size() const noexcept -> vec2u const & { return m_size; }
		
		ML_NODISCARD auto width() const noexcept -> size_t { return m_size[0]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD auto begin() noexcept -> iterator { return m_pixels.begin(); }
		
		ML_NODISCARD auto begin() const noexcept -> const_iterator { return m_pixels.begin(); }
		
		ML_NODISCARD auto cbegin() const noexcept -> const_iterator { return m_pixels.cbegin(); }

		ML_NODISCARD auto cend() const noexcept -> const_iterator { return m_pixels.cend(); }

		ML_NODISCARD auto crbegin() const noexcept -> const_reverse_iterator { return m_pixels.crbegin(); }

		ML_NODISCARD auto crend() const noexcept -> const_reverse_iterator { return m_pixels.crend(); }

		ML_NODISCARD auto end() noexcept -> iterator { return m_pixels.end(); }
		
		ML_NODISCARD auto end() const noexcept -> const_iterator { return m_pixels.end(); }

		ML_NODISCARD auto rbegin() noexcept -> reverse_iterator { return m_pixels.rbegin(); }
		
		ML_NODISCARD auto rbegin() const noexcept -> const_reverse_iterator { return m_pixels.rbegin(); }

		ML_NODISCARD auto rend() noexcept -> reverse_iterator { return m_pixels.rend(); }
		
		ML_NODISCARD auto rend() const noexcept -> const_reverse_iterator { return m_pixels.rend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec2u			m_size;
		size_t			m_channels;
		storage_type	m_pixels;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD inline auto make_image(Args && ... args)
	{
		return image{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_IMAGE_HPP_