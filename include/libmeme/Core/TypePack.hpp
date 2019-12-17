#ifndef _ML_TYPE_PACK_HPP_
#define _ML_TYPE_PACK_HPP_

namespace ml::detail
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args> struct pack
	{
		template <template <class...> class T> struct apply
		{
			using type = typename T<Args...>;
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		template <class...> class To,
		template <template <class...> class> class T
	> struct unpack
	{
		using type = typename T<To>::type;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		template <class...> class To,
		template <template <class...> class> class T
	> using unpack_t = typename unpack<To, T>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PACK_HPP_