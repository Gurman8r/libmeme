#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace util
	{
		template <class To, class From
		> ML_NODISCARD constexpr tvec4<To> color_cast(const tvec4<From> & value)
		{
			return (tvec4<To>)value;
		}

		ML_NODISCARD constexpr vec4b color_cast(vec4f const & value)
		{
			return (vec4b)(value * 255.f);
		}

		ML_NODISCARD constexpr vec4f color_cast(vec4b const & value)
		{
			return ((vec4f)value) / 255.f;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class _T> struct ML_NODISCARD basic_color final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type	= typename _T;
		using rgb_type		= typename tvec3<value_type>;
		using rgba_type		= typename tvec4<value_type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr basic_color(rgba_type const & value)
			: m_value{ value }
		{
		}

		constexpr basic_color(rgb_type const & rgb, value_type a)
			: m_value{ rgb[0], rgb[1], rgb[2], a }
		{
		}

		constexpr basic_color(value_type rgba)
			: m_value{ rgba, rgba, rgba, rgba }
		{
		}

		constexpr basic_color(value_type r, value_type g, value_type b)
			: m_value{ r, g, b, 1 }
		{
		}

		constexpr basic_color(value_type r, value_type g, value_type b, value_type a)
			: m_value{ r, g, b, a }
		{
		}

		template <class U> constexpr basic_color(tvec4<U> const & value)
			: m_value{ util::color_cast(value) }
		{
		}

		template <class U> constexpr basic_color(basic_color<U> const & value)
			: m_value{ util::color_cast(value.rgba()) }
		{
		}

		constexpr basic_color()
			: m_value{ 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		operator rgba_type & () & noexcept { return m_value; }

		operator rgba_type && () && noexcept { return std::move(m_value); }

		constexpr operator rgba_type const & () const & noexcept { return m_value; }

		constexpr operator rgba_type const && () const && noexcept { return std::move(m_value); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr value_type & operator[](size_t i) & noexcept { return m_value[i]; }

		constexpr value_type const & operator[](size_t i) const & noexcept { return m_value[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto r() const  noexcept-> value_type const & { return m_value[0]; }
		
		constexpr auto g() const  noexcept-> value_type const & { return m_value[1]; }
		
		constexpr auto b() const  noexcept-> value_type const & { return m_value[2]; }
		
		constexpr auto a() const  noexcept-> value_type const & { return m_value[3]; }
		
		constexpr auto rgb() const  noexcept-> rgb_type { return (rgb_type)m_value; }
		
		constexpr auto rgba() const & noexcept-> rgba_type const & { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		rgba_type m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_alias color = typename basic_color<float_t>;
	
	ML_alias color32 = typename basic_color<byte_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> std::ostream & operator<<(std::ostream & out, basic_color<T> const & value)
	{
		return out << value.rgba();
	}

	template <class T
	> std::istream & operator>>(std::istream & in, basic_color<T> & value)
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

		static constexpr auto clear		= color{ 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr auto white		= color{ 1.0f, 1.0f, 1.0f, 1.0f };
		static constexpr auto gray		= color{ 0.5f, 0.5f, 0.5f, 1.0f };
		static constexpr auto black		= color{ 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr auto red		= color{ 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr auto green		= color{ 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr auto blue		= color{ 0.0f, 0.0f, 1.0f, 1.0f };
		static constexpr auto cyan		= color{ 0.0f, 1.0f, 1.0f, 1.0f };
		static constexpr auto yellow	= color{ 1.0f, 1.0f, 0.0f, 1.0f };
		static constexpr auto magenta	= color{ 1.0f, 0.0f, 1.0f, 1.0f };
		static constexpr auto violet	= color{ 0.5f, 0.0f, 1.0f, 1.0f };
		static constexpr auto lime		= color{ 0.5f, 1.0f, 0.0f, 1.0f };
		static constexpr auto orange	= color{ 1.0f, 0.5f, 0.0f, 1.0f };
		static constexpr auto fuchsia	= color{ 1.0f, 0.0f, 0.5f, 1.0f };
		static constexpr auto aqua		= color{ 0.0f, 1.0f, 0.5f, 1.0f };
		static constexpr auto azure		= color{ 0.0f, 0.5f, 1.0f, 1.0f };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLOR_HPP_