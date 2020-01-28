#ifndef _ML_TYPE_LIST_HPP_
#define _ML_TYPE_LIST_HPP_

// Sources:
// https://www.youtube.com/watch?v=NTWSeQtHZ9M
// https://github.com/SuperV1234/cppcon2015
// https://github.com/SuperV1234/cppcon2015/tree/master/Other/ecs/Utils/MPL
// https://github.com/SuperV1234/cppcon2015/blob/master/Other/ecs/Utils/MetaFor.hpp
// https://github.com/SuperV1234/cppcon2015/blob/master/Other/ecs/Utils/MPL/TypeListOps.hpp
// https://stackoverflow.com/questions/18063451/get-index-of-a-tuple-elements-type
// https://stackoverflow.com/questions/25958259/how-do-i-find-out-if-a-tuple-contains-a-type

#include <libmeme/Common.hpp>

#define ML_FWD(v)   std::forward<decltype(v)>(v)
#define ML_TYPE(v)  typename decltype(v)::type

namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts
	> struct type_list { static constexpr size_t size{ sizeof...(Ts) }; };

	template <class T
	> struct type_tag { using type = T; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Loops
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		template <class Fn, class Tuple, size_t ... Indices
		> constexpr decltype(auto) tuple_apply_impl(Fn && fn, Tuple && tp, std::index_sequence<Indices...>)
		{
			return ML_FWD(fn)(std::get<Indices>(ML_FWD(tp))...);
		}

		template <class Fn, class Tuple
		> constexpr decltype(auto) tuple_apply(Fn && fn, Tuple && tp)
		{
			return meta::impl::tuple_apply_impl(
				ML_FWD(fn),
				ML_FWD(tp),
				std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{}
			);
		}

		template <class Fn, class ... Args
		> constexpr decltype(auto) for_args(Fn && fn, Args && ... args)
		{
			return (void)std::initializer_list<int>{ (fn(ML_FWD(args)), 0)... };
		}

		template <class Fn, class Tuple
		> constexpr decltype(auto) for_tuple(Fn && fn, Tuple && tp)
		{
			return meta::impl::tuple_apply([&fn](auto && ... vs)
			{
				meta::impl::for_args(fn, ML_FWD(vs)...);
			},
			ML_FWD(tp));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn, class Tuple
	> constexpr decltype(auto) tuple_apply(Fn && fn, Tuple && tp)
	{
		return meta::impl::tuple_apply(fn, ML_FWD(tp));
	}

	template <class Fn, class ... Args
	> constexpr decltype(auto) for_args(Fn && fn, Args && ... args)
	{
		return meta::impl::for_args(fn, ML_FWD(args)...);
	}

	template <class Fn, class Tuple
	> constexpr decltype(auto) for_tuple(Fn && fn, Tuple && tp)
	{
		return meta::impl::for_tuple(fn, ML_FWD(tp));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// List Operations
namespace ml::meta
{
	// RENAME
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class...> class To, class T
	> struct rename_impl;

	template <
		template <class ...> class To,
		template <class...> class From,
		class ... Ts
	> struct rename_impl<To, From<Ts...>>
	{
		using type = To<Ts...>;
	};

	template<
		template <class...> class To, class T
	> ML_USING rename = typename rename_impl<To, T>::type;

	// CONCAT
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ...
	> struct concat_impl
	{
		using type = type_list<>;
	};

	template <class ... Ts
	> ML_USING concat = typename concat_impl<Ts...>::type;

	template <class ... Ts
	> struct concat_impl<type_list<Ts...>>
	{
		using type = type_list<Ts...>;
	};

	template <class ... Ts0, class ... Ts1, class ... Rest
	> struct concat_impl<type_list<Ts0...>, type_list<Ts1...>, Rest...>
	{
		using type = concat<type_list<Ts0..., Ts1...>, Rest...>;
	};

	// REBIND
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Fn, class
	> struct rebind_impl
	{
		using type = type_list<>;
	};

	template <template <class> class Pr, class Ls
	> ML_USING rebind = typename rebind_impl<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct rebind_impl<Pr, type_list<T, Ts...>>
	{
		using type = concat<
			type_list<Pr<T>>, rebind<Pr, type_list<Ts...>>
		>;
	};

	// INDEX OF
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ls
	> struct index_of;

	template <class T, class ... Ts
	> struct index_of<T, type_list<T, Ts...>>
		: std::integral_constant<size_t, 0> {};

	template <class T, class U, class ... Ts
	> struct index_of<T, type_list<U, Ts...>>
		: std::integral_constant<size_t, 1 + index_of<T, type_list<Ts...>>::value> {};

	// CONTAINS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ls
	> struct contains;

	template <class T
	> struct contains<T, type_list<>>
		: std::false_type {};

	template <class T, class U, class ... Ts
	> struct contains<T, type_list<U, Ts...>>
		: contains<T, type_list<Ts...>> {};

	template <class T, class ... Ts
	> struct contains<T, type_list<T, Ts...>>
		: std::true_type {};

	// GENERAL
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class Ls
	> ML_USING tuple = rename<std::tuple, Ls>;
	
	template <class Ls
	> ML_USING type_tuple = typename tuple<rebind<type_tag, Ls>>;

	template <class Ls, class Fn
	> constexpr void for_types(Fn && fn) noexcept
	{
		return meta::for_tuple(fn, type_tuple<Ls>{});
	}

	template <class Ls
	> constexpr size_t size() noexcept
	{
		return Ls::size;
	}

	template <class Ls, class T> ML_USING push_back = concat<Ls, type_list<T>>;

	template <class Ls, class T> ML_USING push_front = concat<type_list<T>, Ls>;

	template <size_t I, class Ls> ML_USING nth = std::tuple_element_t<I, tuple<Ls>>;

	template <class Ls> ML_USING head = nth<0, Ls>;

	template <class Ls> ML_USING tail = nth<size<Ls>() - 1, Ls>;

	// REPEAT
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t N, class T
	> struct repeat_impl
	{
		using type = push_back<typename repeat_impl<N - 1, T>::type, T>;
	};

	template <class T
	> struct repeat_impl<0, T>
	{
		using type = type_list<>;
	};

	template <size_t N, class T
	> ML_USING repeat = typename repeat_impl<N, T>::type;

	// FILTER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Pr, class
	> struct filter_impl
	{
		using type = type_list<>;
	};

	template <template <class> class Pr, class Ls
	> ML_USING filter = typename filter_impl<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct filter_impl<Pr, type_list<T, Ts...>>
	{
		using next = filter<Pr, type_list<Ts...>>;

		using type = std::conditional_t<
			Pr<T>{},
			concat<type_list<T>, next>,
			next
		>;
	};

	// ALL
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class, class...
	> struct all
		: std::true_type {};

	template <template <class> class Pr, class T, class ... Ts
	> struct all<Pr, T, Ts...>
		: std::bool_constant<(Pr<T>{} && all<Pr, Ts...>{})> {};

	template <template <class> class TMF
	> struct bound_all
	{
		template <class ... Ts
		> using type = all<TMF, Ts...>;
	};

	template <template <class> class TMF, class TL
	> ML_USING all_types = rename<bound_all<TMF>::template type, TL>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_LIST_HPP_