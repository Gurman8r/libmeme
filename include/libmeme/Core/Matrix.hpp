#ifndef _ML_MATRIX_HPP_
#define _ML_MATRIX_HPP_

#include <libmeme/Core/Array.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::ds
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// fixed size matrix
	template <class _Ty, size_t _Width, size_t _Height
	> struct matrix
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static_assert(0 < (_Width * _Height), "size negative or zero");
		
		static_assert(std::is_trivial_v<_Ty>, "type must be trivial");

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename _Ty;
		using self_type			= typename matrix<value_type, _Width, _Height>;
		using storage_type		= typename ds::array<value_type, _Width * _Height>;
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

		ML_NODISCARD constexpr operator storage_type & () noexcept
		{
			return m_data;
		}

		ML_NODISCARD constexpr operator storage_type const & () const noexcept
		{
			return m_data;
		}

		template <class U, size_t W, size_t H
		> ML_NODISCARD constexpr operator matrix<U, W, H>() const noexcept
		{
			if constexpr (std::is_same_v<matrix<U, W, H>, self_type>)
			{
				return (*this);
			}
			else if constexpr (W == _Width && H == _Height)
			{
				auto temp{ matrix<U, W, H>::zero() };
				for (size_t i = 0; i < temp.size(); ++i)
				{
					temp[i] = static_cast<U>(at(i));
				}
				return temp;
			}
			else
			{
				auto temp{ matrix<U, W, H>::zero() };
				for (size_t i = 0; i < temp.size(); ++i)
				{
					size_t const x{ i % temp.width() }, y{ i / temp.width() };
					temp[i] = (y < _Height && x < _Width)
						? static_cast<U>(at(y * _Width + x))
						: static_cast<U>(0);
				}
				return temp;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr auto operator[](size_type const i) -> reference { return m_data[i]; }

		ML_NODISCARD constexpr auto operator[](size_type const i) const -> const_reference { return m_data[i]; }

		ML_NODISCARD constexpr auto at(size_type const i) -> reference { return m_data.at(i); }
		
		ML_NODISCARD constexpr auto at(size_type const i) const -> const_reference { return m_data.at(i); }

		ML_NODISCARD constexpr auto at(size_type const x, size_type const y) -> reference { return at(y * _Width + x); }

		ML_NODISCARD constexpr auto at(size_type const x, size_type const y) const -> const_reference { return at(y * _Width + x); }

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

		ML_NODISCARD static constexpr self_type one() noexcept
		{
			auto temp{ zero() };
			for (auto & elem : temp)
				elem = static_cast<value_type>(1);
			return temp;
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

namespace ml
{
	// MATRIX NxN
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_VA(class T, size_t N) tmatn = ds::matrix<T, N, N>;
	ML_USING_VA(class T, size_t N) tvecn = ds::matrix<T, N, 1>;


	// MATRIX 2x2
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tmat2 = tmatn<X, 2>;
	ML_USING	mat2b = tmat2<byte_t>;
	ML_USING	mat2i = tmat2<int32_t>;
	ML_USING	mat2u = tmat2<uint32_t>;
	ML_USING	mat2f = tmat2<float32_t>;
	ML_USING	mat2d = tmat2<float64_t>;
	ML_USING	mat2s = tmat2<size_t>;
	ML_USING	mat2 = mat2f;


	// MATRIX 3x3
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tmat3 = tmatn<X, 3>;
	ML_USING	mat3b = tmat3<byte_t>;
	ML_USING	mat3i = tmat3<int32_t>;
	ML_USING	mat3u = tmat3<uint32_t>;
	ML_USING	mat3f = tmat3<float32_t>;
	ML_USING	mat3d = tmat3<float64_t>;
	ML_USING	mat3s = tmat3<size_t>;
	ML_USING	mat3 = mat3f;


	// MATRIX 4x4
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tmat4 = tmatn<X, 4>;
	ML_USING	mat4b = tmat4<byte_t>;
	ML_USING	mat4i = tmat4<int32_t>;
	ML_USING	mat4u = tmat4<uint32_t>;
	ML_USING	mat4f = tmat4<float32_t>;
	ML_USING	mat4d = tmat4<float64_t>;
	ML_USING	mat4s = tmat4<size_t>;
	ML_USING	mat4 = mat4f;


	// VECTOR 2
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tvec2 = tvecn<X, 2>;
	ML_USING	vec2b = tvec2<byte_t>;
	ML_USING	vec2i = tvec2<int32_t>;
	ML_USING	vec2u = tvec2<uint32_t>;
	ML_USING	vec2f = tvec2<float32_t>;
	ML_USING	vec2d = tvec2<float64_t>;
	ML_USING	vec2s = tvec2<size_t>;
	ML_USING	vec2 = vec2f;


	// VECTOR 3
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tvec3 = tvecn<X, 3>;
	ML_USING	vec3b = tvec3<byte_t>;
	ML_USING	vec3i = tvec3<int32_t>;
	ML_USING	vec3u = tvec3<uint32_t>;
	ML_USING	vec3f = tvec3<float32_t>;
	ML_USING	vec3d = tvec3<float64_t>;
	ML_USING	vec3s = tvec3<size_t>;
	ML_USING	vec3 = vec3f;


	// VECTOR 4
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_USING_X	tvec4 = tvecn<X, 4>;
	ML_USING	vec4b = tvec4<byte_t>;
	ML_USING	vec4i = tvec4<int32_t>;
	ML_USING	vec4u = tvec4<uint32_t>;
	ML_USING	vec4f = tvec4<float32_t>;
	ML_USING	vec4d = tvec4<float64_t>;
	ML_USING	vec4s = tvec4<size_t>;
	ML_USING	vec4 = vec4f;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t X, size_t Y
	> inline ML_SERIALIZE(std::ostream & out, ds::matrix<T, X, Y> const & value)
	{
		for (auto const & elem : value)
			out << elem << ' ';
		return out;
	}

	template <class T, size_t X, size_t Y
	> inline ML_DESERIALIZE(std::istream & in, ds::matrix<T, X, Y> & value)
	{
		for (auto & elem : value)
			if (in.good())
				in >> elem;
		return in;
	}

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
}

namespace ml
{
	// Vector Arithmetic Operators
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator+=(tvecn<Tx, N> & lhs, tvecn<Ty, N> const & rhs) noexcept
		-> tvecn<Tx, N> &
	{
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			lhs[i] = (lhs[i] + static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator-=(tvecn<Tx, N> & lhs, tvecn<Ty, N> const & rhs) noexcept
		-> tvecn<Tx, N> &
	{
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			lhs[i] = (lhs[i] - static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator*=(tvecn<Tx, N> & lhs, tvecn<Ty, N> const & rhs) noexcept
		-> tvecn<Tx, N> &
	{
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			lhs[i] = (lhs[i] * static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator/=(tvecn<Tx, N> & lhs, tvecn<Ty, N> const & rhs) noexcept
		-> tvecn<Tx, N> &
	{
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			lhs[i] = (lhs[i] / static_cast<Tx>(rhs[i]));
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator+(tvecn<Tx, N> const & lhs, tvecn<Ty, N> const & rhs) noexcept
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp{ lhs };
		return temp += rhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator-(tvecn<Tx, N> const & lhs, tvecn<Ty, N> const & rhs) noexcept
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp{ lhs };
		return temp -= rhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator*(tvecn<Tx, N> const & lhs, tvecn<Ty, N> const & rhs) noexcept
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp{ lhs };
		return temp *= rhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator/(tvecn<Tx, N> const & lhs, tvecn<Ty, N> const & rhs) noexcept
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp{ lhs };
		return temp /= rhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator+=(tvecn<Tx, N> & lhs, Ty const & rhs) noexcept
		-> tvecn<Tx, N> &
	{
		for (auto & elem : lhs)
		{
			elem += static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator-=(tvecn<Tx, N> & lhs, Ty const & rhs) noexcept
		-> tvecn<Tx, N> &
	{
		for (auto & elem : lhs)
		{
			elem -= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator*=(tvecn<Tx, N> & lhs, Ty const & rhs) noexcept
		-> tvecn<Tx, N> &
	{
		for (auto & elem : lhs)
		{
			elem *= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator/=(tvecn<Tx, N> & lhs, Ty const & rhs) noexcept
		-> tvecn<Tx, N> &
	{
		for (auto & elem : lhs)
		{
			elem /= static_cast<Tx>(rhs);
		}
		return lhs;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator+(tvecn<Tx, N> const & lhs, Ty const & rhs) noexcept
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp{ lhs };
		return temp += static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator-(tvecn<Tx, N> const & lhs, Ty const & rhs) noexcept
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp{ lhs };
		return temp -= static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator*(tvecn<Tx, N> const & lhs, Ty const & rhs) noexcept
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp{ lhs };
		return temp *= static_cast<Tx>(rhs);
	}

	template <class Tx, class Ty, size_t N
	> ML_NODISCARD constexpr auto operator/(tvecn<Tx, N> const & lhs, Ty const & rhs) noexcept
		-> tvecn<Tx, N>
	{
		tvecn<Tx, N> temp{ lhs };
		return temp /= static_cast<Tx>(rhs);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, size_t N
	> ML_NODISCARD constexpr auto operator-(tvecn<T, N> const & lhs) noexcept
		-> tvecn<T, N>
	{
		return (lhs * static_cast<T>(-1));
	}

	template <class T, size_t N
	> ML_NODISCARD constexpr auto operator+(tvecn<T, N> const & lhs) noexcept
		-> tvecn<T, N>
	{
		return -(-(lhs));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATRIX_HPP_