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
		using pixels_type				= typename pmr::vector<byte_t>;
		using iterator					= typename pixels_type::iterator;
		using const_iterator			= typename pixels_type::const_iterator;
		using reverse_iterator			= typename pixels_type::reverse_iterator;
		using const_reverse_iterator	= typename pixels_type::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image() noexcept : image{ allocator_type{} }
		{
		}

		explicit image(allocator_type alloc) noexcept
			: m_size	{ 0 }
			, m_channels{ 0 }
			, m_pixels	{ alloc }
		{
		}

		image(vec2i const & size, size_t channels, pixels_type const & pix, allocator_type alloc = {})
			: m_size	{ size }
			, m_channels{ channels }
			, m_pixels	{ pix, alloc }
		{
		}

		image(vec2i const & size, size_t channels = 4, allocator_type alloc = {}) noexcept
			: m_size	{ size }
			, m_channels{ channels }
			, m_pixels	{ capacity(), 0, alloc }
		{
		}

		image(fs::path const & path, bool flip_v = false, size_t req_channels = 0, allocator_type alloc = {})
			: image{ alloc }
		{
			load_from_file(path, flip_v, req_channels);
		}

		image(image const & value, allocator_type alloc = {})
			: image{ value.m_size, value.m_channels, value.m_pixels, alloc }
		{
		}

		image(image && value, allocator_type alloc = {}) noexcept
			: image{ alloc }
		{
			swap(std::move(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image & operator=(image const & value)
		{
			image temp{ value };
			swap(temp);
			return (*this);
		}

		image & operator=(image && value) noexcept
		{
			swap(std::move(value));
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept
		{
			m_size = {};
			m_channels = 0;
			m_pixels.clear();
		}

		void swap(image & value) noexcept
		{
			if (this != std::addressof(value))
			{
				m_pixels.swap(value.m_pixels);

				m_size.swap(value.m_size);

				std::swap(m_channels, value.m_channels);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static bool read_pixels(fs::path const & path, pixels_type & px, vec2i & sz, size_t & ch, int32_t rc = 4, bool fv = 0);

		bool load_from_file(fs::path const & path, int32_t rc = 0, bool fv = 0) noexcept
		{
			return read_pixels(path, m_pixels, m_size, m_channels, rc, fv);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image & create_from_color(vec2i const & size, color32 const & c) noexcept
		{
			return create_from_color(size, channels(), c);
		}

		image & create_from_color(color32 const & c) noexcept
		{
			return create_from_color(size(), channels(), c);
		}

		image & create_from_color(vec2i const & size, size_t channels, color32 const & c) noexcept
		{
			if (size[0] && size[1] && channels)
			{
				m_size = size;
				m_channels = channels;
				m_pixels.resize(capacity());

				auto it{ begin() };
				while (it != end())
				{
					if (m_channels >= 1) *it++ = c[0];
					if (m_channels >= 2) *it++ = c[1];
					if (m_channels >= 3) *it++ = c[2];
					if (m_channels >= 4) *it++ = c[3];
				}
			}
			else
			{
				clear();
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image & create_from_pixels(vec2i const & size, pixels_type const & pixels) noexcept
		{
			return create_from_pixels(size, m_channels, pixels);
		}

		image & create_from_pixels(pixels_type const & pixels) noexcept
		{
			return create_from_pixels(m_size, m_channels, pixels);
		}

		image & create_from_pixels(vec2i const & size, size_t channels, pixels_type const & pixels) noexcept
		{
			if (!pixels.empty() && (pixels.size() == (size[0] * size[1] * channels)))
			{
				m_size = size;
				m_channels = channels;
				m_pixels = pixels;
			}
			else
			{
				clear();
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		image & flip_vertically() noexcept
		{
			if (!empty())
			{
				size_t const cols{ width() * channels() };
				for (size_t y = 0; y < height(); ++y)
				{
					auto lhs{ begin() + y * cols }, rhs{ begin() + (y + 1) * cols - channels() };
					for (size_t x = 0; x < width() / 2; ++x)
					{
						std::swap_ranges(lhs, lhs + channels(), rhs);
						lhs += channels();
						rhs -= channels();
					}
				}
			}
			return (*this);
		}

		image & flip_horizontally() noexcept
		{
			if (!empty())
			{
				size_t const cols{ width() * channels() };
				auto top{ begin() }, bot{ end() - cols };
				for (size_t y = 0; y < height() / 2; ++y)
				{
					std::swap_ranges(top, top + cols, bot);
					top += cols;
					bot -= cols;
				}
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD std::optional<color32> get_pixel(size_t index) const noexcept
		{
			return (index < capacity())
				? std::make_optional(color32{
					(m_channels >= 1) ? *((cbegin() + index) + 0) : (byte_t)0,
					(m_channels >= 2) ? *((cbegin() + index) + 1) : (byte_t)0,
					(m_channels >= 3) ? *((cbegin() + index) + 2) : (byte_t)0,
					(m_channels >= 4) ? *((cbegin() + index) + 3) : (byte_t)0
					})
				: std::nullopt;
		}

		ML_NODISCARD std::optional<color32> get_pixel(size_t x, size_t y) const noexcept
		{
			return get_pixel((x + y * m_size[0]) * m_channels);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool set_pixel(size_t index, color32 const & c) noexcept
		{
			if (index < capacity())
			{
				auto it{ begin() + index };
				if (m_channels >= 1) *it++ = c[0];
				if (m_channels >= 2) *it++ = c[1];
				if (m_channels >= 3) *it++ = c[2];
				if (m_channels >= 4) *it++ = c[3];
				return true;
			}
			return false;
		}

		bool set_pixel(size_t x, size_t y, color32 const & c) noexcept
		{
			return set_pixel((x + y * m_size[0]) * m_channels, c);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD operator bool() const noexcept { return !this->empty(); }

		ML_NODISCARD auto operator[](size_t i) -> byte_t & { return m_pixels.at(i); }

		ML_NODISCARD auto operator[](size_t i) const -> byte_t const & { return m_pixels.at(i); }

		ML_NODISCARD auto bounds() const noexcept -> uint_rect { return uint_rect { {}, size() }; }
		
		ML_NODISCARD auto capacity() const noexcept -> size_t { return width() * height() * channels(); }
		
		ML_NODISCARD auto channels() const noexcept -> size_t { return m_channels; }

		ML_NODISCARD auto data() noexcept -> byte_t * { return m_pixels.data(); }
		
		ML_NODISCARD auto data() const noexcept -> byte_t const * { return m_pixels.data(); }
		
		ML_NODISCARD bool empty() const noexcept { return m_pixels.empty(); }
		
		ML_NODISCARD auto height() const noexcept -> size_t { return m_size[1]; }

		ML_NODISCARD auto pixels() noexcept -> pixels_type & { return m_pixels; }
		
		ML_NODISCARD auto pixels() const noexcept -> pixels_type const & { return m_pixels; }
		
		ML_NODISCARD auto size() const noexcept -> vec2i const & { return m_size; }
		
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

		static image const & get_default_rgba() noexcept
		{
			static image temp{ vec2i{ 512, 512 }, 3 };
			for (size_t i = 0, w = temp.width(), h = temp.height(); i < w * h; ++i)
			{
				size_t const y{ i % w }, x{ i / w };

				temp.set_pixel(x, y, ((y < h / 2) && (x < w / 2)) || ((y >= h / 2) && (x >= w / 2))
					? color{ color{ 0.1f }.rgb(), 1.0 }
					: (y >= h / 2) || (x >= w / 2) ? colors::magenta : colors::green
				);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		vec2i		m_size;
		size_t		m_channels;
		pixels_type	m_pixels;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_IMAGE_HPP_