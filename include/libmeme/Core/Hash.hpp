#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

#include <libmeme/Core/Core.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// constexpr fnv1a hash implementation
	struct Hash final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto fnv1a_basis { static_cast<hash_t>(14695981039346656037ULL) };

		static constexpr auto fnv1a_prime { static_cast<hash_t>(1099511628211ULL) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args> constexpr Hash(Args && ... args) noexcept
			: m_value{ (*this)(std::forward<Args>(args)...) }
		{
		}

		constexpr Hash() noexcept
			: m_value{ 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T
		> ML_NODISCARD constexpr hash_t operator()(T const * arr, hash_t size, hash_t seed) noexcept
		{
			return ((size > 0)
				? (*this)((arr + 1), (size - 1), (seed ^ static_cast<hash_t>(*arr)) * fnv1a_prime)
				: seed
			);
		}

		template <class T
		> ML_NODISCARD constexpr hash_t operator()(T const * arr, hash_t size) noexcept
		{
			return (*this)(arr, size, fnv1a_basis);
		}

		template <class T, hash_t N
		> ML_NODISCARD constexpr hash_t operator()(const T(&value)[N]) noexcept
		{
			return (*this)(value, (N - 1));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Arr
		> ML_NODISCARD constexpr hash_t operator()(Arr const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		template <template <class...> class Arr, class ... Ts
		> ML_NODISCARD constexpr hash_t operator()(Arr<Ts...> const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ML_NODISCARD constexpr hash_t operator()() const noexcept { return m_value; }

		ML_NODISCARD constexpr operator hash_t() const noexcept { return (*this)(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		union { hash_t m_value; };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_