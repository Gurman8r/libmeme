#ifndef _ML_TRANSFORM_HPP_
#define _ML_TRANSFORM_HPP_

// WIP

#include <libmeme/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct transform final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr transform() noexcept
			: m_mtx{ mat4::identity() }
		{
		}
		
		template <class Pos, class Rot, class Scl
		> constexpr transform(Pos && p, Rot && r, Scl && s) noexcept
			: transform{}
		{
			(*this)
				.set_position(ML_FWD(p))
				.set_rotation(ML_FWD(r))
				.set_scale(ML_FWD(s));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr transform(mat4 const & value)
			: m_mtx{ value }
		{
		}

		constexpr transform(mat3 const & value) noexcept
			: m_mtx{ (mat4)value }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr transform(transform const & other)
			: transform{ other.m_mtx }
		{
		}

		constexpr transform(transform && other) noexcept
			: transform{ std::move(other.m_mtx) }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr transform & operator=(transform const & other)
		{
			transform temp{ other };
			swap(temp);
			return (*this);
		}

		constexpr transform & operator=(transform && other) noexcept
		{
			swap(std::move(other));
			return (*this);
		}

		constexpr void swap(transform & other) noexcept
		{
			if (this != std::addressof(other))
			{
				util::swap(m_mtx, other.m_mtx);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr mat4 & matrix() noexcept { return m_mtx; }

		constexpr mat4 const & matrix() const noexcept { return m_mtx; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr vec4 get_col(size_t const i) const noexcept
		{
			return {
				m_mtx.at(0, i),
				m_mtx.at(1, i),
				m_mtx.at(2, i),
				m_mtx.at(3, i)
			};
		}

		constexpr transform & set_col(size_t const i, vec4 const & v) noexcept
		{
			m_mtx.at(0, i) = v[0];
			m_mtx.at(1, i) = v[1];
			m_mtx.at(2, i) = v[2];
			m_mtx.at(3, i) = v[3];
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr vec4 get_row(size_t const i) const noexcept
		{
			return {
				m_mtx.at(i, 0),
				m_mtx.at(i, 1),
				m_mtx.at(i, 2),
				m_mtx.at(i, 3)
			};
		}

		constexpr transform & set_row(size_t const i, vec4 const & v) noexcept
		{
			m_mtx.at(i, 0) = v[0];
			m_mtx.at(i, 1) = v[1];
			m_mtx.at(i, 2) = v[2];
			m_mtx.at(i, 3) = v[3];
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr vec3 left() const noexcept { return get_row(0); }
		
		constexpr vec3 up() const noexcept { return get_row(1); }
		
		constexpr vec3 forward() const noexcept { return get_row(2); }
		
		constexpr vec3 translation() const noexcept { return get_row(3); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr transform & left(vec3 const & v) noexcept { return set_row(0, v); }
		
		constexpr transform & up(vec3 const & v) noexcept { return set_row(1, v); }
		
		constexpr transform & forward(vec3 const & v) noexcept { return set_row(2, v); }
		
		constexpr transform & translation(vec3 const & v) noexcept { return set_row(3, v); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr transform & translate(vec3 const & v) noexcept
		{
			vec3 const temp{ translation() };
			return translation(temp + (temp * v));
		}

		constexpr transform & rotate(vec3 const & v, float_t const angle) noexcept
		{
			float_t const a{ angle };
			float_t const c{ gcem::cos(a) };
			float_t const s{ gcem::sin(a) };

			vec3 axis{ util::normalize(v) };

			vec3 temp{ axis * (1.0f - c) };

			transform out{};

			// WIP

			return (*this) = out;
		}

		constexpr transform & scale(vec3 const & v) noexcept
		{
			return (*this)
				.set_col(0, get_col(0) * v[0])
				.set_col(1, get_col(1) * v[1])
				.set_col(2, get_col(2) * v[2])
				.set_col(3, get_col(3));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr vec3 get_position() const noexcept
		{
			return translation();
		}

		constexpr vec4 get_rotation() const noexcept
		{
			return vec4{ .0f, .1f, .0f, .25f };
		}

		constexpr vec3 get_scale() const noexcept
		{
			return vec3::one();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr transform & set_position(vec3 const & v) noexcept
		{
			return translation(v);
		}

		constexpr transform & set_rotation(vec4 const & v) noexcept
		{
			return (*this);
		}

		constexpr transform & set_scale(vec3 const & v) noexcept
		{
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		mat4 m_mtx;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> ML_NODISCARD static constexpr auto make_transform(Args && ... args) noexcept
	{
		return transform{ ML_FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRANSFORM_HPP_