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

#define ECS_FWD(x) std::forward<decltype(x)>(x)

#define ECS_TYPE(mX) typename decltype(mX)::type

namespace ml::meta
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace impl
	{
		template <class Fn, class Tuple, size_t ... Indices
		> constexpr decltype(auto) tuple_apply_impl(Fn && fn, Tuple && tp, std::index_sequence<Indices...>)
		{
			return ECS_FWD(fn)(std::get<Indices>(ECS_FWD(tp))...);
		}

		template <class Fn, class Tuple
		> constexpr decltype(auto) tuple_apply(Fn && fn, Tuple && tp)
		{
			return tuple_apply_impl(ECS_FWD(fn), ECS_FWD(tp),
				std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{}
			);
		}

		template <class Fn, class ... Args
		> constexpr decltype(auto) for_args(Fn && fn, Args && ... args)
		{
			return (void)std::initializer_list<int>{ (fn(ECS_FWD(args)), 0)... };
		}

		template <class Fn, class Tuple
		> constexpr decltype(auto) for_tuple(Fn && fn, Tuple && tp)
		{
			return tuple_apply([&fn](auto && ... vs) {
				for_args(fn, ECS_FWD(vs)...);
			}, ECS_FWD(tp));
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn, class Tuple
	> constexpr decltype(auto) tuple_apply(Fn && fn, Tuple && tp)
	{
		return impl::tuple_apply(fn, ECS_FWD(tp));
	}

	template <class Fn, class ... Args
	> constexpr decltype(auto) for_args(Fn && fn, Args && ... args)
	{
		return impl::for_args(fn, ECS_FWD(args)...);
	}

	template <class Fn, class Tuple
	> constexpr decltype(auto) for_tuple(Fn && fn, Tuple && tp)
	{
		return impl::for_tuple(fn, ECS_FWD(tp));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml::meta
{
	// TYPE LIST
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts
	> struct type_list
	{
		static constexpr size_t size{ sizeof...(Ts) };
	};

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
		using type = typename To<Ts...>;
	};

	template<
		template <class...> class To, class T
	> using rename = typename rename_impl<To, T>::type;

	// CONCAT
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ...
	> struct concat_impl
	{
		using type = type_list<>;
	};

	template <class ... Ts
	> using concat = typename concat_impl<Ts...>::type;

	template <class ... Ts
	> struct concat_impl<type_list<Ts...>>
	{
		using type = type_list<Ts...>;
	};

	template <class ... Ts0, class ... Ts1, class ... Rest
	> struct concat_impl<type_list<Ts0..., Ts1...>, Rest...>
	{
		using type = concat<type_list<Ts0..., Ts1...>, Rest...>;
	};

	// MAP
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Fn, class
	> struct map_impl
	{
		using type = type_list<>;
	};

	template <template <class> class Fn, class List
	> using map = typename map_impl<Fn, List>::type;

	template <template <class> class Fn, class T, class ... Ts
	> struct map_impl<Fn, type_list<T, Ts...>>
	{
		using type = concat<type_list<Fn<T>>, map<Fn, type_list<Ts...>>>;;
	};

	// INDEX OF
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class List
	> struct index_of;

	template <class T, class ... Ts
	> struct index_of<T, type_list<T, Ts...>> : std::integral_constant<
		size_t, 0
	>{};

	template <class T, class U, class ... Ts
	> struct index_of<T, type_list<U, Ts...>> : std::integral_constant<
		size_t, 1 + index_of<T, type_list<Ts...>>::value
	>{};

	// CONTAINS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class List
	> struct contains;

	template <class T
	> struct contains<T, type_list<>> : std::false_type {};

	template <class T, class U, class ... Ts
	> struct contains<T, type_list<U, Ts...>> : contains<T, type_list<Ts...>> {};

	template <class T, class ... Ts
	> struct contains<T, type_list<T, Ts...>> : std::true_type {};

	// OPERATIONS
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> struct type_tag { using type = T; };

	template <class List
	> using tuple = rename<std::tuple, List>;

	template <class List
	> using type_tuple = meta::tuple<map<type_tag, List>>;

	template <size_t Index, class List
	> using Nth = std::tuple_element_t<Index, meta::tuple<List>>;

	template <class List, class T
	> using push_back = concat<List, type_list<T>>;

	template <class List, class T
	> using push_front = concat<type_list<T>, List>;

	template <class List
	> static constexpr size_t size() noexcept
	{
		return List::size;
	}

	template <class List, class Fn
	> constexpr void for_types(Fn && fn) noexcept
	{
		return for_tuple(fn, type_tuple<List>{});
	}

	template <class List
	> using head = Nth<0, List>;

	template <class List
	> using tail = Nth<meta::size<List>() - 1, List>;

	// REPEAT
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <size_t N, class T
	> struct repeat_impl
	{
		using type = push_back<
			typename repeat_impl<N - 1, T>::type, T
		>;
	};

	template <class T
	> struct repeat_impl<0, T>
	{
		using type = type_list<>;
	};

	template <size_t N, class T
	> using repeat = typename repeat_impl<N, T>::type;

	// FILTER
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <template <class> class Pr, class
	> struct filter_impl
	{
		using type = type_list<>;
	};

	template <template <class> class Pr, class List
	> using filter = typename filter_impl<Pr, List>::type;

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
	> struct all : std::true_type {};

	template <template <class> class Pr, class T, class ... Ts
	> struct all<Pr, T, Ts...> : std::bool_constant<(
		Pr<T>{} && all<Pr, Ts...>{}
	)>{};

	template <template <class> class TMF
	> struct bound_all
	{
		template <class ... Ts
		> using type = all<TMF, Ts...>;
	};

	template <template <class> class TMF, class TL
	> using all_types = rename<bound_all<TMF>::template type, TL>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TYPE_LIST_HPP_