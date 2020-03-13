#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <libmeme/Core/Array.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// MATRIX
namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// fixed size matrix
	template <class _Ty, size_t _Width, size_t _Height
	> struct matrix
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(
			0 < (_Width * _Height),
			"matrix size negative or zero"
		);

		static_assert(
			std::is_integral_v<_Ty> || std::is_floating_point_v<_Ty>,
			"matrix only supports integral and floating point types"
		);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename _Ty;
		using self_type			= typename matrix<value_type, _Width, _Height>;
		using storage_type		= typename array<value_type, _Width * _Height>;
		using size_type			= typename storage_type::size_type;
		using difference_type	= typename storage_type::difference_type;
		using pointer			= typename storage_type::pointer;
		using reference			= typename storage_type::reference;
		using const_pointer		= typename storage_type::const_pointer;
		using const_reference	= typename storage_type::const_reference;
		using iterator			= typename storage_type::iterator;
		using const_iterator	= typename storage_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		storage_type m_data; // aggregate initializer

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator storage_type & () & noexcept { return m_data; }

		ML_NODISCARD constexpr operator storage_type const & () const & noexcept { return m_data; }

		ML_NODISCARD constexpr operator storage_type && () && noexcept { return std::move(m_data); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U, size_t W, size_t H
		> ML_NODISCARD constexpr operator matrix<U, W, H>() const noexcept
		{
			if constexpr (std::is_same_v<matrix<U, W, H>, self_type>)
			{
				return (*this); // same type
			}
			else
			{
				auto temp{ matrix<U, W, H>::zero() };
				if constexpr (W == _Width && H == _Height)
				{
					// same dimensions
					for (size_t i = 0; i < (W * H); ++i)
					{
						temp[i] = (U)at(i);
					}
				}
				else
				{
					// different dimensions
					for (size_t i = 0; i < (W * H); ++i)
					{
						size_t const x{ i % W }, y{ i / W };
						temp[i] = (y < _Height && x < _Width) ? (U)at(x, y) : (U)0;
					}
				}
				return temp;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr operator pointer() noexcept { return (pointer)m_data; }

		ML_NODISCARD constexpr operator const_pointer() const noexcept { return (const_pointer)m_data; }

		ML_NODISCARD constexpr auto operator*() noexcept -> reference { return (*m_data); }

		ML_NODISCARD constexpr auto operator*() const noexcept -> const_reference { return (*m_data); }

		ML_NODISCARD constexpr auto at(size_type const i) -> reference { return m_data.at(i); }
		
		ML_NODISCARD constexpr auto at(size_type const i) const -> const_reference { return m_data.at(i); }

		ML_NODISCARD constexpr auto at(size_type const x, size_type const y) -> reference { return m_data.at(y * _Width + x); }

		ML_NODISCARD constexpr auto at(size_type const x, size_type const y) const -> const_reference { return m_data.at(y * _Width + x); }

		ML_NODISCARD constexpr auto back() noexcept -> reference { return m_data.back(); }
		
		ML_NODISCARD constexpr auto back() const noexcept -> const_reference { return m_data.back(); }
		
		ML_NODISCARD constexpr auto begin() noexcept -> iterator { return m_data.begin(); }
		
		ML_NODISCARD constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		
		ML_NODISCARD constexpr auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		
		ML_NODISCARD constexpr auto cend() const noexcept -> const_iterator { return m_data.cend(); }

		ML_NODISCARD constexpr auto data() noexcept -> pointer { return m_data.data(); }
		
		ML_NODISCARD constexpr auto data() const noexcept -> const_pointer { return m_data.data(); }

		ML_NODISCARD constexpr bool empty() const noexcept { return false; }
		
		ML_NODISCARD constexpr auto end() noexcept -> iterator { return m_data.end(); }
		
		ML_NODISCARD constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }
		
		ML_NODISCARD constexpr auto front() noexcept -> reference { return m_data.front(); }
		
		ML_NODISCARD constexpr auto front() const noexcept -> const_reference { return m_data.front(); }

		ML_NODISCARD constexpr auto height() const noexcept -> size_t { return _Height; }

		ML_NODISCARD constexpr auto max_size() const noexcept -> size_t { return m_data.max_size(); }

		ML_NODISCARD constexpr auto size() const noexcept -> size_t { return m_data.size(); }

		ML_NODISCARD constexpr auto width() const noexcept -> size_t { return _Width; }
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD static constexpr self_type zero() noexcept
		{
			return self_type{ 0 };
		}

		ML_NODISCARD static constexpr self_type fill(value_type const v) noexcept
		{
			auto temp{ zero() };
			for (auto & e : temp)
				e = v;
			return temp;
		}

		ML_NODISCARD static constexpr self_type one() noexcept
		{
			return self_type::fill(value_type{ 1 });
		}

		ML_NODISCARD static constexpr self_type identity() noexcept
		{
			auto temp{ zero() };
			for (size_t i = 0; i < temp.size(); ++i)
			{
				temp[i] = (i / temp.width()) == (i % temp.width())
					? static_cast<value_type>(1)
					: static_cast<value_type>(0);
			}
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// ALIASES
namespace ml
{
	// BASE
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class T, size_t N
	> ML_ALIAS tmatnxn = ds::matrix<T, N, N>;

	template <class T, size_t N
	> ML_ALIAS tvector = ds::matrix<T, N, 1>;


	// MATRIX 2x2
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class T
	> ML_ALIAS	tmat2 = tmatnxn<T, 2>;
	ML_ALIAS	mat2b = tmat2<byte_t>;
	ML_ALIAS	mat2i = tmat2<int32_t>;
	ML_ALIAS	mat2u = tmat2<uint32_t>;
	ML_ALIAS	mat2f = tmat2<float32_t>;
	ML_ALIAS	mat2d = tmat2<float64_t>;
	ML_ALIAS	mat2s = tmat2<size_t>;
	ML_ALIAS	mat2 = mat2f;


	// MATRIX 3x3
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class T
	> ML_ALIAS	tmat3 = tmatnxn<T, 3>;
	ML_ALIAS	mat3b = tmat3<byte_t>;
	ML_ALIAS	mat3i = tmat3<int32_t>;
	ML_ALIAS	mat3u = tmat3<uint32_t>;
	ML_ALIAS	mat3f = tmat3<float32_t>;
	ML_ALIAS	mat3d = tmat3<float64_t>;
	ML_ALIAS	mat3s = tmat3<size_t>;
	ML_ALIAS	mat3 = mat3f;


	// MATRIX 4x4
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class T
	> ML_ALIAS	tmat4 = tmatnxn<T, 4>;
	ML_ALIAS	mat4b = tmat4<byte_t>;
	ML_ALIAS	mat4i = tmat4<int32_t>;
	ML_ALIAS	mat4u = tmat4<uint32_t>;
	ML_ALIAS	mat4f = tmat4<float32_t>;
	ML_ALIAS	mat4d = tmat4<float64_t>;
	ML_ALIAS	mat4s = tmat4<size_t>;
	ML_ALIAS	mat4 = mat4f;


	// VECTOR 2
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class T
	> ML_ALIAS	tvec2 = tvector<T, 2>;
	ML_ALIAS	vec2b = tvec2<byte_t>;
	ML_ALIAS	vec2i = tvec2<int32_t>;
	ML_ALIAS	vec2u = tvec2<uint32_t>;
	ML_ALIAS	vec2f = tvec2<float32_t>;
	ML_ALIAS	vec2d = tvec2<float64_t>;
	ML_ALIAS	vec2s = tvec2<size_t>;
	ML_ALIAS	vec2 = vec2f;


	// VECTOR 3
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class T
	> ML_ALIAS	tvec3 = tvector<T, 3>;
	ML_ALIAS	vec3b = tvec3<byte_t>;
	ML_ALIAS	vec3i = tvec3<int32_t>;
	ML_ALIAS	vec3u = tvec3<uint32_t>;
	ML_ALIAS	vec3f = tvec3<float32_t>;
	ML_ALIAS	vec3d = tvec3<float64_t>;
	ML_ALIAS	vec3s = tvec3<size_t>;
	ML_ALIAS	vec3 = vec3f;


	// VECTOR 4
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class T
	> ML_ALIAS	tvec4 = tvector<T, 4>;
	ML_ALIAS	vec4b = tvec4<byte_t>;
	ML_ALIAS	vec4i = tvec4<int32_t>;
	ML_ALIAS	vec4u = tvec4<uint32_t>;
	ML_ALIAS	vec4f = tvec4<float32_t>;
	ML_ALIAS	vec4d = tvec4<float64_t>;
	ML_ALIAS	vec4s = tvec4<size_t>;
	ML_ALIAS	vec4 = vec4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// MATRIX OPERATORS
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr bool operator==(ds::matrix<Tx, W, H> const & lhs, ds::matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data == rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr bool operator!=(ds::matrix<Tx, W, H> const & lhs, ds::matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data != rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr bool operator<(ds::matrix<Tx, W, H> const & lhs, ds::matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data < rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr bool operator<=(ds::matrix<Tx, W, H> const & lhs, ds::matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data <= rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr bool operator>(ds::matrix<Tx, W, H> const & lhs, ds::matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data > rhs.m_data);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr bool operator>=(ds::matrix<Tx, W, H> const & lhs, ds::matrix<Ty, W, H> const & rhs) noexcept
	{
		return (lhs.m_data >= rhs.m_data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator+=(ds::matrix<Tx, W, H> & lhs, Ty const rhs) noexcept
		-> ds::matrix<Tx, W, H> &
	{
		for (auto & elem : lhs)
		{
			elem += static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator-=(ds::matrix<Tx, W, H> & lhs, Ty const rhs) noexcept
		-> ds::matrix<Tx, W, H> &
	{
		for (auto & elem : lhs)
		{
			elem -= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator*=(ds::matrix<Tx, W, H> & lhs, Ty const rhs) noexcept
		-> ds::matrix<Tx, W, H> &
	{
		for (auto & elem : lhs)
		{
			elem *= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator/=(ds::matrix<Tx, W, H> & lhs, Ty const rhs) noexcept
		-> ds::matrix<Tx, W, H> &
	{
		for (auto & elem : lhs)
		{
			elem /= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator+(ds::matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
		-> ds::matrix<Tx, W, H>
	{
		auto temp{ lhs };
		return temp += static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator-(ds::matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
		-> ds::matrix<Tx, W, H>
	{
		auto temp{ lhs };
		return temp -= static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator*(ds::matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
		-> ds::matrix<Tx, W, H>
	{
		auto temp{ lhs };
		return temp *= static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator/(ds::matrix<Tx, W, H> const & lhs, Ty const rhs) noexcept
		-> ds::matrix<Tx, W, H>
	{
		auto temp{ lhs };
		return temp /= static_cast<Tx>(rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator-(ds::matrix<T, W, H> const & lhs) noexcept
		-> ds::matrix<T, W, H>
	{
		return (lhs * static_cast<T>(-1));
	}

	template <class T, size_t W, size_t H
	> ML_NODISCARD constexpr auto operator+(ds::matrix<T, W, H> const & lhs) noexcept
		-> ds::matrix<T, W, H>
	{
		return -(-(lhs));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// VECTOR OPERATORS
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator+=(tvector<Tx, N> & lhs, tvector<Ty, N> const & rhs) noexcept
		-> tvector<Tx, N> &
	{
		for (size_t i = 0; i < N; ++i)
		{
			lhs[i] = (lhs[i] + static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator-=(tvector<Tx, N> & lhs, tvector<Ty, N> const & rhs) noexcept
		-> tvector<Tx, N> &
	{
		for (size_t i = 0; i < N; ++i)
		{
			lhs[i] = (lhs[i] - static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator*=(tvector<Tx, N> & lhs, tvector<Ty, N> const & rhs) noexcept
		-> tvector<Tx, N> &
	{
		for (size_t i = 0; i < N; ++i)
		{
			lhs[i] = (lhs[i] * static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator/=(tvector<Tx, N> & lhs, tvector<Ty, N> const & rhs) noexcept
		-> tvector<Tx, N> &
	{
		for (size_t i = 0; i < N; ++i)
		{
			lhs[i] = (lhs[i] / static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator+(tvector<Tx, N> const & lhs, tvector<Ty, N> const & rhs) noexcept
		-> tvector<Tx, N>
	{
		auto temp{ lhs };
		return temp += rhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator-(tvector<Tx, N> const & lhs, tvector<Ty, N> const & rhs) noexcept
		-> tvector<Tx, N>
	{
		auto temp{ lhs };
		return temp -= rhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator*(tvector<Tx, N> const & lhs, tvector<Ty, N> const & rhs) noexcept
		-> tvector<Tx, N>
	{
		auto temp{ lhs };
		return temp *= rhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator/(tvector<Tx, N> const & lhs, tvector<Ty, N> const & rhs) noexcept
		-> tvector<Tx, N>
	{
		auto temp{ lhs };
		return temp /= rhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// TUPLE INTERFACE
namespace std
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t I, class T, size_t W, size_t H
	> ML_NODISCARD constexpr T & get(_ML ds::matrix<T, W, H> & value) noexcept
	{
		return std::get<I>(value.m_data);
	}

	template <size_t I, class T, size_t W, size_t H
	> ML_NODISCARD constexpr T const & get(_ML ds::matrix<T, W, H> const & value) noexcept
	{
		return std::get<I>(value.m_data);
	}

	template <size_t I, class T, size_t W, size_t H
	> ML_NODISCARD constexpr T && get(_ML ds::matrix<T, W, H> && value) noexcept
	{
		return std::get<I>(value.m_data);
	}

	template <size_t I, class T, size_t W, size_t H
	> ML_NODISCARD constexpr T const && get(_ML ds::matrix<T, W, H> const && value) noexcept
	{
		return std::get<I>(value.m_data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> ML_NODISCARD constexpr T & get(_ML ds::matrix<T, W, H> & value) noexcept
	{
		return std::get<Y * W + X>(value);
	}

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> ML_NODISCARD constexpr T const & get(_ML ds::matrix<T, W, H> const & value) noexcept
	{
		return std::get<Y * W + X>(value);
	}

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> ML_NODISCARD constexpr T && get(_ML ds::matrix<T, W, H> && value) noexcept
	{
		return std::get<Y * W + X>(value);
	}

	template <size_t X, size_t Y, class T, size_t W, size_t H
	> ML_NODISCARD constexpr T const && get(_ML ds::matrix<T, W, H> const && value) noexcept
	{
		return std::get<Y * W + X>(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// JSON INTERFACE
namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H
	> static void to_json(json & j, matrix<T, W, H> const & value)
	{
		j = value.m_data;
	}

	template <class T, size_t W, size_t H
	> static void from_json(json const & j, matrix<T, W, H> & value)
	{
		j.get_to(value.m_data);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// STREAM INTERFACE
namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t W, size_t H
	> inline ML_serialize(std::ostream & out, matrix<T, W, H> const & value)
	{
		for (auto const & e : value)
			if (out.good())
				out << value << ' ';
		return out;
	}

	template <class T, size_t W, size_t H
	> inline ML_deserialize(std::istream & in, matrix<T, W, H> & value)
	{
		for (auto & e : value)
			if (in.good())
				in >> e;
		return in;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_MATRIX_HPP_