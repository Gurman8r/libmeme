#ifndef _ML_HASH_HPP_
#define _ML_HASH_HPP_

#include <libmeme/Core/Core.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// fnv1a hash implementation
	struct Hash final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr auto fnv1a_basis { static_cast<hash_t>(14695981039346656037ULL) };

		static constexpr auto fnv1a_prime { static_cast<hash_t>(1099511628211ULL) };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class ... Args> constexpr Hash(Args && ... args) noexcept
			: m_value { (*this)(std::forward<Args>(args)...) }
		{
		}

		constexpr Hash() noexcept : m_value { 0 } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> constexpr hash_t operator()(T const * arr, hash_t size, hash_t seed) noexcept
		{
			return ((size > 0)
				? (*this)((arr + 1), (size - 1), (seed ^ static_cast<hash_t>(*arr)) * fnv1a_prime)
				: seed
			);
		}

		template <
			class T
		> constexpr hash_t operator()(T const * arr, hash_t size) noexcept
		{
			return (*this)(arr, size, fnv1a_basis);
		}

		template <
			class T, hash_t N
		> constexpr hash_t operator()(const T(&value)[N]) noexcept
		{
			return (*this)(value, (N - 1));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Arr
		> constexpr hash_t operator()(Arr const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		template <
			template <class> class Arr, class Ch
		> constexpr hash_t operator()(Arr<Ch> const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		template <
			template <class, class> class Arr, class Ch, class Tr
		> constexpr hash_t operator()(Arr<Ch, Tr> const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		template <
			template <class, class, class> class Arr, class Ch, class Tr, class Al
		> constexpr hash_t operator()(Arr<Ch, Tr, Al> const & value) noexcept
		{
			return (*this)(value.data(), value.size());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr hash_t const & operator()() const noexcept
		{
			return m_value;
		}

		constexpr operator hash_t const &() const noexcept
		{
			return (*this)();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: hash_t m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_HASH_HPP_
