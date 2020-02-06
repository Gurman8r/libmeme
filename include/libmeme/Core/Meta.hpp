#ifndef _ML_META_HPP_
#define _ML_META_HPP_

// Sources:
// https://www.youtube.com/watch?v=NTWSeQtHZ9M
// https://github.com/SuperV1234/cppcon2015
// https://github.com/SuperV1234/cppcon2015/tree/master/Other/ecs/Utils/MPL
// https://github.com/SuperV1234/cppcon2015/blob/master/Other/ecs/Utils/MetaFor.hpp
// https://github.com/SuperV1234/cppcon2015/blob/master/Other/ecs/Utils/MPL/TypeListOps.hpp
// 
// https://stackoverflow.com/questions/18063451/get-index-of-a-tuple-elements-type
// https://stackoverflow.com/questions/25958259/how-do-i-find-out-if-a-tuple-contains-a-type
// https://stackoverflow.com/questions/37029886/how-to-construct-a-tuple-from-an-array
// https://stackoverflow.com/questions/36580855/construct-tuple-by-passing-the-same-argument-to-each-element-with-explicit-const
// https://stackoverflow.com/questions/22560100/how-to-initialize-all-tuple-elements-by-the-same-arguments

#include <libmeme/Common.hpp>

// DS
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts
	> struct list { static constexpr size_t size{ sizeof...(Ts) }; };

	template <class T
	> struct tag { using type = typename T; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// Loops
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Fn, class Tup, size_t ... I
		> constexpr decltype(auto) tuple_apply_impl(Fn && fn, Tup && tp, std::index_sequence<I...>)
		{
			return ML_FWD(fn)(std::get<I>(ML_FWD(tp))...);
		}

		template <class Fn, class Tup
		> constexpr decltype(auto) tuple_apply(Fn && fn, Tup && tp)
		{
			return meta::impl::tuple_apply_impl(
				ML_FWD(fn),
				ML_FWD(tp),
				std::make_index_sequence<std::tuple_size_v<std::decay_t<Tup>>>{}
			);
		}

		template <class Fn, class ... Args
		> constexpr decltype(auto) for_args(Fn && fn, Args && ... args)
		{
			return (void)std::initializer_list<int>{ (fn(ML_FWD(args)), 0)... };
		}

		template <class Fn, class Tup
		> constexpr decltype(auto) for_tuple(Fn && fn, Tup && tp)
		{
			return meta::impl::tuple_apply([&fn](auto && ... vs)
			{
				meta::impl::for_args(fn, ML_FWD(vs)...);
			},
			ML_FWD(tp));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn, class Tup
	> constexpr decltype(auto) tuple_apply(Fn && fn, Tup && tp)
	{
		return meta::impl::tuple_apply(fn, ML_FWD(tp));
	}

	template <class Fn, class ... Args
	> constexpr decltype(auto) for_args(Fn && fn, Args && ... args)
	{
		return meta::impl::for_args(fn, ML_FWD(args)...);
	}

	template <class Fn, class Tup
	> constexpr decltype(auto) for_tuple(Fn && fn, Tup && tp)
	{
		return meta::impl::for_tuple(fn, ML_FWD(tp));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// CONCAT
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ...
	> struct concat_impl
	{
		using type = typename list<>;
	};

	template <class ... Ts
	> ML_USING concat = typename concat_impl<Ts...>::type;

	template <class ... Ts
	> struct concat_impl<list<Ts...>>
	{
		using type = typename list<Ts...>;
	};

	template <class ... Ts0, class ... Ts1, class ... Rest
	> struct concat_impl<list<Ts0...>, list<Ts1...>, Rest...>
	{
		using type = typename concat<list<Ts0..., Ts1...>, Rest...>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// REMAP - modify inner type
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Fn, class
	> struct remap_impl
	{
		using type = typename list<>;
	};

	template <template <class> class Pr, class Ls
	> ML_USING remap = typename remap_impl<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct remap_impl<Pr, list<T, Ts...>>
	{
		using type = typename concat<
			list<Pr<T>>, remap<Pr, list<Ts...>>
		>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// RENAME - modify outer type
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class...> class To, class T
	> struct rename_impl;

	template <
		template <class ...> class To,
		template <class...> class From,
		class ... Ts
	> struct rename_impl<To, From<Ts...>>
	{
		using type = typename To<Ts...>;
	};

	template<
		template <class...> class To, class T
	> ML_USING rename = typename rename_impl<To, T>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// CONTAINS - list contains type
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ls
	> struct contains;

	template <class T
	> struct contains<T, list<>>
		: std::false_type {};

	template <class T, class U, class ... Ts
	> struct contains<T, list<U, Ts...>>
		: contains<T, list<Ts...>> {};

	template <class T, class ... Ts
	> struct contains<T, list<T, Ts...>>
		: std::true_type {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// INDEX OF - index of type in list
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ls
	> struct index_of;

	template <class T, class ... Ts
	> struct index_of<T, list<T, Ts...>>
		: std::integral_constant<size_t, 0>
	{
		static_assert(contains<T, list<T, Ts...>>(),
			"type not found in list");
	};

	template <class T, class U, class ... Ts
	> struct index_of<T, list<U, Ts...>>
		: std::integral_constant<size_t, 1 + index_of<T, list<Ts...>>::value>
	{
		static_assert(contains<T, list<U, Ts...>>(),
			"type not found in list");
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// GENERAL
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class Ls
	> ML_USING tuple = typename rename<std::tuple, Ls>;
	
	template <class Ls
	> ML_USING type_tuple = typename tuple<remap<tag, Ls>>;

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

	template <class Ls, class T> ML_USING push_back = typename concat<Ls, list<T>>;

	template <class Ls, class T> ML_USING push_front = typename concat<list<T>, Ls>;

	template <size_t I, class Ls> ML_USING nth = typename std::tuple_element_t<I, tuple<Ls>>;

	template <class Ls> ML_USING head = typename nth<0, Ls>;

	template <class Ls> ML_USING tail = typename nth<size<Ls>() - 1, Ls>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// REPEAT - size N list of T
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t N, class T
	> struct repeat_impl
	{
		using type = typename push_back<typename repeat_impl<N - 1, T>::type, T>;
	};

	template <class T
	> struct repeat_impl<0, T>
	{
		using type = typename list<>;
	};

	template <size_t N, class T
	> ML_USING repeat = typename repeat_impl<N, T>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// FILTER - remove types from list
namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Pr, class
	> struct filter_impl
	{
		using type = typename list<>;
	};

	template <template <class> class Pr, class Ls
	> ML_USING filter = typename filter_impl<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct filter_impl<Pr, list<T, Ts...>>
	{
		using next = typename filter<Pr, list<Ts...>>;

		using type = typename std::conditional_t<
			Pr<T>{},
			concat<list<T>, next>,
			next
		> ;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ALL - used to run a check against all types in a list
namespace ml::meta
{
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
		> using type = typename all<TMF, Ts...>;
	};

	template <template <class> class TMF, class TL
	> ML_USING all_types = typename rename<bound_all<TMF>::template type, TL>;

	// example
	static_assert(all<std::is_const, const int>{});
	static_assert(all_types<std::is_const, list<const int>>{});

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_META_HPP_