#ifndef _ML_STRING_UTILITY_HPP_
#define _ML_STRING_UTILITY_HPP_

#include <libmeme/Core/Utility.hpp>

#define ML_PMR_STRING_TEMPLATE(C, T, A, S)				\
	class C = char,										\
	class T = std::char_traits<C>,						\
	class A = pmr::polymorphic_allocator<_ML byte_t>,	\
	class S = pmr::basic_string<C, T>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch
	> struct is_char : std::bool_constant<
		util::is_any_of_v<Ch, char, wchar_t, char16_t, char32_t>
	> {};

	template <class Ch
	> ML_alias is_char_t = typename _ML util::is_char<Ch>;

	template <class Ch
	> static constexpr bool is_char_v{ _ML util::is_char_t<Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch = char
	> struct is_cstring : std::bool_constant<
		_ML util::is_char_v<Ch> &&
		std::is_convertible_v<T const &, Ch const *>
	> {};

	template <class T, class Ch = char
	> ML_alias is_cstring_t = typename _ML util::is_cstring<T, Ch>;

	template <class T, class Ch = char
	> static constexpr bool is_cstring_v{ _ML util::is_cstring_t<T, Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch = char, class Traits = std::char_traits<Ch>
	> struct is_string_view : std::bool_constant<
		_ML util::is_cstring_v<T, Ch> ||
		std::is_convertible_v<T const &, std::basic_string_view<Ch, Traits>>
	> {};

	template <class T, class Ch = char
	> ML_alias is_string_view_t = typename _ML util::is_string_view<T, Ch>;

	template <class T, class Ch = char
	> static constexpr bool is_string_view_v{ _ML util::is_string_view_t<T, Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch = char
	> struct is_string : std::bool_constant<
		util::is_any_of_v<T, std::basic_string<Ch>, pmr::basic_string<Ch>>
	> {};

	template <class T, class Ch = char
	> ML_alias is_string_t = typename _ML util::is_string<T, Ch>;

	template <class T, class Ch = char
	> static constexpr bool is_string_v{ _ML util::is_string_t<T, Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch, class Fn, class ... Args
	> ML_NODISCARD inline auto parse_answer(Ch const * ptr, Fn && fn, Args && ... args) noexcept
	{
		Ch * end{};
		auto const answer{ std::invoke(ML_forward(fn), ptr, &end, ML_forward(args)...) };
		if (!(*end != 0 || end == ptr))
		{
			return std::make_optional<T>(static_cast<T>(answer));
		}
		else
		{
			return (std::optional<T>)std::nullopt;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch, class T
	> ML_NODISCARD inline pmr::basic_string<Ch> integral_to_string(T const value) noexcept
	{
		static_assert(std::is_integral_v<T>);

		using		U		= typename std::make_unsigned_t<T>;
		Ch			buf[21]	{};
		Ch * const	end		{ std::end(buf) };
		Ch *		next	{ end };
		auto const	uval	{ static_cast<U>(value) };

		auto uint_to_string = [](Ch * next, auto uval)
		{
			if constexpr (sizeof(U) > 4)
			{
				while (uval > 0xFFFFFFFFU)
				{
					auto chunk{ static_cast<ulong_t>(uval % 1000000000) };
					uval /= 1000000000;
					for (int32_t i = 0; i != 9; ++i)
					{
						*--next = static_cast<Ch>('0' + chunk % 10);
						chunk /= 10;
					}
				}
			}
			auto trunc{ static_cast<ulong_t>(uval) };
			do {
				*--next = static_cast<Ch>('0' + trunc % 10);
				trunc /= 10;
			} while (trunc != 0);
			return next;
		};

		if (value < 0)
		{
			next = uint_to_string(next, 0 - uval);
			*--next = '-';
		}
		else
		{
			next = uint_to_string(next, uval);
		}
		return pmr::basic_string<Ch>{ next, end };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch, class T
	> ML_NODISCARD inline pmr::basic_string<Ch> floating_point_to_string(T const value) noexcept
	{
		static_assert(std::is_floating_point_v<T>);
		auto const len{ static_cast<size_t>(_CSTD _scprintf("%f", value)) };
		pmr::basic_string<Ch> str{ len, 0, pmr::polymorphic_allocator<byte_t>{} };
		_CSTD sprintf_s(str.data(), len + 1, "%f", value);
		return str;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Arg0, class ... Args
	> ML_NODISCARD inline std::stringstream args_to_sstream(Arg0 && arg0, Args && ... args) noexcept
	{
		std::stringstream ss{};
		ss << ML_forward(arg0) << '\n';
		int32_t i[] = { 0, ((void)(ss << args << '\n'), 0)... }; (void)i;
		return ss;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> ML_NODISCARD constexpr size_t strlen(Ch const * value)
	{
		return ((*value) ? (1 + _ML util::strlen(value + 1)) : 0);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline auto widen(Str const & value) noexcept
	{
		pmr::wstring temp{};
		temp.reserve(value.size());
		for (auto const c : value)
			temp.push_back(static_cast<wchar_t>(c));
		return temp;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline auto narrow(Str const & value) noexcept
	{
		pmr::string temp{};
		temp.reserve(value.size());
		for (auto const c : value)
			temp.push_back(static_cast<char>(c));
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD inline pmr::vector<pmr::string> tokenize(pmr::string value, pmr::string const & delim) noexcept
	{
		if (value.empty()) return {};
		if (delim.empty()) return { value };

		pmr::vector<pmr::string> temp{};
		size_t i{};
		while ((i = value.find(delim)) != value.npos)
		{
			temp.push_back(value.substr(0, i));
			
			value.erase(0, i + delim.size());
		}
		temp.push_back(value);
		return temp;
	}

	ML_NODISCARD inline pmr::string detokenize(pmr::vector<pmr::string> const & value, pmr::string const & delim = " ")
	{
		if (value.empty())
			return {};
		if (value.size() == 1)
			return value.front();
		std::stringstream ss{};
		for (auto const & str : value)
		{
			ss << str << delim;
		}
		return pmr::string{ ss.str() };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> ML_NODISCARD constexpr bool is_whitespace(Ch const c) noexcept
	{
		switch (static_cast<char>(c))
		{
		case '\n':
		case '\t':
		case '\r':
		case ' ' : return true;
		default  : return false;
		}
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str trim_front(Str value) noexcept
	{
		while (!value.empty() && is_whitespace(value.front()))
		{
			value.erase(value.begin());
		}
		return value;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str trim_back(Str value) noexcept
	{
		while (!value.empty() && is_whitespace(value.back()))
		{
			value.pop_back();
		}
		return value;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str trim(Str value) noexcept
	{
		return trim_front(trim_back(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str replace_all(Str value, Str const & toFind, Str const & replaceWith) noexcept
	{
		if (!value.empty() && !toFind.empty())
		{
			for (size_t i = 0; (i = value.find(toFind, i)) != Str::npos;)
			{
				value.replace(i, toFind.size(), replaceWith);
				i += replaceWith.size();
			}
		}
		return value;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str remove_all(Str value, Str const & toRemove) noexcept
	{
		return replace_all(value, toRemove, {});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_alpha(Str const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::isalpha(elem, loc))
				return false;
		return true;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_alnum(Str const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::isalnum(elem, loc))
				return false;
		return true;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_graph(Str const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::isgraph(elem, loc))
				return false;
		return true;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_print(Str const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::isprint(elem, loc))
				return false;
		return true;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_lower(Str const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::islower(elem, loc))
				return false;
		return true;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_upper(Str const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::islower(elem, loc))
				return false;
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_lower(Str value) noexcept
	{
		std::locale loc{};
		for (auto & elem : value)
			elem = std::tolower(elem, loc);
		return value;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_upper(Str value) noexcept
	{
		std::locale loc{};
		for (auto & elem : value)
			elem = std::toupper(elem, loc);
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_bool(Str const & value) noexcept
	{
		switch (hash(to_lower(value)))
		{
		case hash("1"):
		case hash("true"):
		case hash("on"):
		case hash("yes"):
		case hash("0"):
		case hash("false"):
		case hash("off"):
		case hash("no"):
			return true;
		default:
			return false;
		}
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_integer(Str const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		auto it{ std::begin(value) };
		if ((*it) == '-') it++; 
		while (it != std::end(value) && std::isdigit(*it, loc)) ++it;
		return (it == std::end(value));
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_decimal(Str const & value) noexcept
	{
		if (value.empty()) return false;
		char * endptr{ nullptr };
		auto temp { std::strtod(value.c_str(), &endptr) };
		return !(*endptr != 0 || endptr == value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline bool is_name(Str const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		auto it{ std::begin(value) };
		if (!(*it == '_') || !std::isalpha(*it, loc))
			return false;
		++it;
		std::for_each(it, std::end(value), [&, loc](auto const c)
		{
			if ((c != '_') || !std::isalnum(c, loc))
			{
				return false;
			}
		});
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<bool> to_bool(Str && str) noexcept
	{
		switch (hash(to_lower(ML_forward(str))))
		{
		case hash("1"):
		case hash("true"):
		case hash("on"):
		case hash("yes"):
			return std::make_optional(true);
		case hash("0"):
		case hash("false"):
		case hash("off"):
		case hash("no"):
			return std::make_optional(false);
		default:
			return std::nullopt;
		}
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<int8_t> to_i8(Str && str, int32_t base = 10) noexcept
	{
		return _ML util::parse_answer<int8_t>(ML_forward(str).c_str(), &_CSTD strtol, base);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<int16_t> to_i16(Str && str, int32_t base = 10) noexcept
	{
		return _ML util::parse_answer<int16_t>(ML_forward(str).c_str(), &_CSTD strtol, base);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<int32_t> to_i32(Str && str, int32_t base = 10) noexcept
	{
		return _ML util::parse_answer<int32_t>(ML_forward(str).c_str(), &_CSTD strtol, base);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<int64_t> to_i64(Str && str, int32_t base = 10) noexcept
	{
		return _ML util::parse_answer<int64_t>(ML_forward(str).c_str(), &_CSTD strtoll, base);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<uint8_t> to_u8(Str && str, int32_t base = 10) noexcept
	{
		return _ML util::parse_answer<uint8_t>(ML_forward(str).c_str(), &_CSTD strtoul, base);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<uint16_t> to_u16(Str && str, int32_t base = 10) noexcept
	{
		return _ML util::parse_answer<uint16_t>(ML_forward(str).c_str(), &_CSTD strtoul, base);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<uint32_t> to_u32(Str && str, int32_t base = 10) noexcept
	{
		return _ML util::parse_answer<uint32_t>(ML_forward(str).c_str(), &_CSTD strtoul, base);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<uint64_t> to_u64(Str && str, int32_t base = 10) noexcept
	{
		return _ML util::parse_answer<uint64_t>(ML_forward(str).c_str(), &_CSTD strtoull, base);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<float32_t> to_f32(Str && str) noexcept
	{
		return _ML util::parse_answer<float32_t>(ML_forward(str).c_str(), &_CSTD strtod);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<float64_t> to_f64(Str && str) noexcept
	{
		return _ML util::parse_answer<float64_t>(ML_forward(str).c_str(), &_CSTD strtod);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline std::optional<float80_t> to_f80(Str && str) noexcept
	{
		return _ML util::parse_answer<float80_t>(ML_forward(str).c_str(), &_CSTD strtold);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline auto const & to_string(Str const & str)
	{
		return str;
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(int8_t const value) noexcept
	{
		return _ML util::integral_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(int16_t const value) noexcept
	{
		return _ML util::integral_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(int32_t const value) noexcept
	{
		return _ML util::integral_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(int64_t const value) noexcept
	{
		return _ML util::integral_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(uint8_t const value) noexcept
	{
		return _ML util::integral_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(uint16_t const value) noexcept
	{
		return _ML util::integral_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(uint32_t const value) noexcept
	{
		return _ML util::integral_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(uint64_t const value) noexcept
	{
		return _ML util::integral_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(float64_t const value) noexcept
	{
		return _ML util::floating_point_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(float32_t const value) noexcept
	{
		return _ML util::floating_point_to_string<Ch>(value);
	}

	template <ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_string(float80_t const value) noexcept
	{
		return _ML util::floating_point_to_string<Ch>(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Arg0, class ... Args
	> ML_NODISCARD inline pmr::string format(pmr::string const & fmt, Arg0 const & arg0, Args && ... args) noexcept
	{
		std::stringstream ss{ _ML util::args_to_sstream(ML_forward(arg0), ML_forward(args)...) };
		return format(fmt, ss);
	}

	ML_NODISCARD inline pmr::string format(pmr::string fmt, std::stringstream & ss) noexcept
	{
		for (size_t i = 0; ss.good(); ++i)
		{
			pmr::string line;
			if (std::getline(ss, line))
			{
				auto const token = ([i]()
				{
					std::stringstream temp{};
					temp << '{' << i << '}';
					return pmr::string{ temp.str() };
				})();

				for (size_t j = 0; (j = fmt.find(token, j)) != fmt.npos;)
				{
					fmt.replace(j, token.size(), line);

					j += line.size();
				}
			}
		}
		return fmt;
	}

	template <class Str
	> ML_NODISCARD inline pmr::string format(pmr::string fmt, pmr::vector<Str> const & args) noexcept
	{
		for (size_t i = 0, imax = args.size(); i < imax; ++i)
		{
			auto const token = ([i]()
			{
				std::stringstream temp{};
				temp << '{' << i << '}';
				return pmr::string{ temp.str() };
			})();

			for (size_t j = 0; (j = fmt.find(token, j)) != fmt.npos;)
			{
				fmt.replace(j, token.size(), args[i]);

				j += args[i].size();
			}
		}
		return fmt;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_hex(T const & value)
	{
		std::stringstream ss{};
		ss << std::hex << value;
		return ss.str();
	}

	template <class T, ML_PMR_STRING_TEMPLATE(Ch, Tr, Al, Str)
	> ML_NODISCARD inline Str to_oct(T const & value)
	{
		std::stringstream ss{};
		ss << std::oct << value;
		return ss.str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STRING_UTILITY_HPP_