#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		template <
			class To, class From
		> static constexpr tvec4<To> color_cast(const tvec4<From> & value)
		{
			return (tvec4<To>)value;
		}

		static constexpr vec4b color_cast(vec4f const & value)
		{
			return {
				static_cast<byte_t>(value[0] * 255.f),
				static_cast<byte_t>(value[1] * 255.f),
				static_cast<byte_t>(value[2] * 255.f),
				static_cast<byte_t>(value[3] * 255.f)
			};
		}

		static constexpr vec4f color_cast(vec4b const & value)
		{
			return {
				static_cast<float_t>(value[0]) / 255.f,
				static_cast<float_t>(value[1]) / 255.f,
				static_cast<float_t>(value[2]) / 255.f,
				static_cast<float_t>(value[3]) / 255.f
			};
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct basic_color final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type		= typename T;
		using rgb_t		= typename tvec3<type>;
		using rgba_t	= typename tvec4<type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr basic_color(rgba_t const & value)
			: m_value{ value }
		{
		}

		constexpr basic_color(rgb_t const & rgb, type a)
			: m_value{ rgb[0], rgb[1], rgb[2], a }
		{
		}

		constexpr basic_color(type rgba)
			: m_value{ rgba, rgba, rgba, rgba }
		{
		}

		constexpr basic_color(type r, type g, type b)
			: m_value{ r, g, b, 1 }
		{
		}

		constexpr basic_color(type r, type g, type b, type a)
			: m_value{ r, g, b, a }
		{
		}

		template <class U> constexpr basic_color(tvec4<U> const & value)
			: m_value{ detail::color_cast(value) }
		{
		}

		template <class U> constexpr basic_color(basic_color<U> const & other)
			: m_value{ detail::color_cast(other.rgba()) }
		{
		}

		constexpr basic_color()
			: m_value{ 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator rgba_t &() { return m_value; }

		constexpr operator rgba_t const &() const { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline type & operator[](size_t i) { return m_value[i]; }

		constexpr type const & operator[](size_t i) const { return m_value[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto r() const -> type const & { return m_value[0]; }
		
		constexpr auto g() const -> type const & { return m_value[1]; }
		
		constexpr auto b() const -> type const & { return m_value[2]; }
		
		constexpr auto a() const -> type const & { return m_value[3]; }
		
		constexpr auto rgb() const -> rgb_t { return (rgb_t)m_value; }
		
		constexpr auto rgba() const -> rgba_t const & { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		rgba_t m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_ALIAS color = typename basic_color<float_t>;
	
	ML_ALIAS color32 = typename basic_color<byte_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> inline ML_serialize(std::ostream & out, basic_color<T> const & value)
	{
		return out << value.rgba();
	}

	template <class T> inline ML_deserialize(std::istream & in, basic_color<T> & value)
	{
		return in >> value.rgba();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_color(Args && ... args)
	{
		return color{ ML_forward(args)... };
	}

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_color32(Args && ... args)
	{
		return color32{ ML_forward(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class colors final
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto clear		= make_color(0.0f, 0.0f, 0.0f, 0.0f);
		static constexpr auto white		= make_color(1.0f, 1.0f, 1.0f, 1.0f);
		static constexpr auto gray		= make_color(0.5f, 0.5f, 0.5f, 1.0f);
		static constexpr auto black		= make_color(0.0f, 0.0f, 0.0f, 1.0f);
		static constexpr auto red		= make_color(1.0f, 0.0f, 0.0f, 1.0f);
		static constexpr auto green		= make_color(0.0f, 1.0f, 0.0f, 1.0f);
		static constexpr auto blue		= make_color(0.0f, 0.0f, 1.0f, 1.0f);
		static constexpr auto cyan		= make_color(0.0f, 1.0f, 1.0f, 1.0f);
		static constexpr auto yellow	= make_color(1.0f, 1.0f, 0.0f, 1.0f);
		static constexpr auto magenta	= make_color(1.0f, 0.0f, 1.0f, 1.0f);
		static constexpr auto violet	= make_color(0.5f, 0.0f, 1.0f, 1.0f);
		static constexpr auto lime		= make_color(0.5f, 1.0f, 0.0f, 1.0f);
		static constexpr auto orange	= make_color(1.0f, 0.5f, 0.0f, 1.0f);
		static constexpr auto fuchsia	= make_color(1.0f, 0.0f, 0.5f, 1.0f);
		static constexpr auto aqua		= make_color(0.0f, 1.0f, 0.5f, 1.0f);
		static constexpr auto azure		= make_color(0.0f, 0.5f, 1.0f, 1.0f);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLOR_HPP_