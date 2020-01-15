#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Matrix.hpp>
#include <libmeme/Core/Core.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		template <class To, class From
		> ML_NODISCARD constexpr tvec4<To> color_cast(const tvec4<From> & value)
		{
			return (tvec4<To>)value;
		}

		ML_NODISCARD constexpr vec4b color_cast(vec4f const & value)
		{
			return {
				static_cast<byte_t>(value[0] * 255.f),
				static_cast<byte_t>(value[1] * 255.f),
				static_cast<byte_t>(value[2] * 255.f),
				static_cast<byte_t>(value[3] * 255.f)
			};
		}

		ML_NODISCARD constexpr vec4f color_cast(vec4b const & value)
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
	
	template <class T> struct BasicColor final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type		= typename T;
		using rgb_t		= typename tvec3<type>;
		using rgba_t	= typename tvec4<type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BasicColor(rgba_t const & value)
			: m_value { value }
		{
		}

		constexpr BasicColor(rgb_t const & rgb, type a)
			: m_value { rgb[0], rgb[1], rgb[2], a }
		{
		}

		constexpr BasicColor(type rgba)
			: m_value { rgba, rgba, rgba, rgba }
		{
		}

		constexpr BasicColor(type r, type g, type b)
			: m_value { r, g, b, 1.0f }
		{
		}

		constexpr BasicColor(type r, type g, type b, type a)
			: m_value { r, g, b, a }
		{
		}

		template <class U> constexpr BasicColor(const tvec4<U> & value)
			: m_value { detail::color_cast(value) }
		{
		}

		template <class U> constexpr BasicColor(const BasicColor<U> & copy)
			: m_value { detail::color_cast(copy.rgba()) }
		{
		}

		constexpr BasicColor()
			: m_value { 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator rgba_t &() { return m_value; }

		ML_NODISCARD constexpr operator rgba_t const &() const { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr type & operator[](size_t i) { return m_value[i]; }

		ML_NODISCARD constexpr type const & operator[](size_t i) const { return m_value[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto r() const -> type const &	{ return m_value[0]; }
		
		ML_NODISCARD constexpr auto g() const -> type const &	{ return m_value[1]; }
		
		ML_NODISCARD constexpr auto b() const -> type const &	{ return m_value[2]; }
		
		ML_NODISCARD constexpr auto a() const -> type const &	{ return m_value[3]; }
		
		ML_NODISCARD constexpr auto rgb() const -> const rgb_t	{ return (rgb_t)m_value; }
		
		ML_NODISCARD constexpr auto rgba() const -> rgba_t const & { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: rgba_t m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_USING Color = typename BasicColor<float_t>;
	
	ML_USING Color32 = typename BasicColor<byte_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> inline ML_SERIALIZE(std::ostream & out, const BasicColor<T> & value)
	{
		return out << value.rgba();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD constexpr auto make_color(Args && ... args)
	{
		return Color{ std::forward<Args>(args)... };
	}

	template <class ... Args
	> ML_NODISCARD constexpr auto make_color32(Args && ... args)
	{
		return Color32{ std::forward<Args>(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace colors
	{
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