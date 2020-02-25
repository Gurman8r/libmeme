#ifndef _ML_STRING_UTILITY_HPP_
#define _ML_STRING_UTILITY_HPP_

#include <libmeme/Core/Utility.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// STRING TRAITS
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch
	> struct is_char : std::bool_constant<
		std::_Is_any_of_v<Ch, char, wchar_t, char16_t, char32_t>
	> {};

	template <class Ch
	> ML_USING is_char_t = typename _ML_UTIL is_char<Ch>;

	template <class Ch
	> static constexpr bool is_char_v{ _ML_UTIL is_char_t<Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch = char
	> struct is_cstring : std::bool_constant<
		_ML_UTIL is_char_v<Ch> &&
		std::is_convertible_v<T const &, Ch const *>
	> {};

	template <class T, class Ch = char
	> ML_USING is_cstring_t = typename _ML_UTIL is_cstring<T, Ch>;

	template <class T, class Ch = char
	> static constexpr bool is_cstring_v{ _ML_UTIL is_cstring_t<T, Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch = char, class Traits = std::char_traits<Ch>
	> struct is_string_view : std::bool_constant<
		_ML_UTIL is_cstring_v<T, Ch> ||
		std::is_convertible_v<T const &, std::basic_string_view<Ch, Traits>>
	> {};

	template <class T, class Ch = char
	> ML_USING is_string_view_t = typename _ML_UTIL is_string_view<T, Ch>;

	template <class T, class Ch = char
	> static constexpr bool is_string_view_v{ _ML_UTIL is_string_view_t<T, Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch = char
	> struct is_string : std::bool_constant<
		std::_Is_any_of_v<T, std::basic_string<Ch>, pmr::basic_string<Ch>>
	> {};

	template <class T, class Ch = char
	> ML_USING is_string_t = typename _ML_UTIL is_string<T, Ch>;

	template <class T, class Ch = char
	> static constexpr bool is_string_v{ _ML_UTIL is_string_t<T, Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// STRING IMPL
namespace ml::util::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Str = pmr::string
	> ML_NODISCARD static inline auto c_string(Str && str) noexcept
	{
		if constexpr (is_cstring_v<decltype(str)>)
		{
			return ML_FWD(str);
		}
		else if constexpr (is_string_v<Str>)
		{
			return ML_FWD(str).c_str();
		}
		else
		{
			static_assert(is_string_view_v<Str>);
			return c_string(pmr::string{ ML_FWD(str) });
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Fn, class ... Args
	> ML_NODISCARD static inline std::optional<T> string_convert(cstring ptr, Fn && fn, Args && ... args) noexcept
	{
		char * end{};
		auto const ans{ std::invoke(ML_FWD(fn), ptr, &end, ML_FWD(args)...) };
		if (!(*end != '\0' || end == ptr))
		{
			return std::make_optional<T>(static_cast<T>(ans));
		}
		else
		{
			return std::nullopt;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch, class T, class = std::enable_if_t<std::is_integral_v<T>>
	> ML_NODISCARD static inline pmr::basic_string<Ch> integral_to_string(T const value) noexcept
	{
		using		U		= typename std::make_unsigned_t<T>;
		Ch			buf[21]	{};
		Ch * const	end		{ _STD end(buf) };
		Ch *		next	{ end };
		auto const	uval	{ static_cast<U>(value) };

		auto uint_to_string = [](Ch * next, auto uval)
		{
			if constexpr (sizeof(U) > 4)
			{
				while (uval > 0xFFFFFFFFU)
				{
					auto chunk{ static_cast<unsigned long>(uval % 1000000000) };
					uval /= 1000000000;
					for (int32_t i = 0; i != 9; ++i)
					{
						*--next = static_cast<Ch>('0' + chunk % 10);
						chunk /= 10;
					}
				}
			}
			auto trunc{ static_cast<unsigned long>(uval) };
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

	template <class Ch, class T, class = std::enable_if_t<std::is_floating_point_v<T>>
	> ML_NODISCARD static inline pmr::basic_string<Ch> floating_point_to_string(T const value) noexcept
	{
		auto const len{ static_cast<size_t>(_CSTD _scprintf("%f", value)) };
		pmr::basic_string<Ch> str{ len, '\0', pmr::polymorphic_allocator<byte_t>{} };
		_CSTD sprintf_s(str.data(), len + 1, "%f", value);
		return str;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Arg0, class ... Args
	> inline std::stringstream sink_stream(Arg0 && arg0, Args && ... args) noexcept
	{
		std::stringstream ss{};
		ss << ML_FWD(arg0) << '\n';
		int32_t i[] = { 0, ((void)(ss << args << '\n'), 0)... }; (void)i;
		return ss;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// STRING UTILITY
namespace ml::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline pmr::vector<pmr::string> tokenize(pmr::string value, pmr::string const & delim) noexcept
	{
		pmr::vector<pmr::string> out;
		size_t pos = 0;
		while ((pos = value.find(delim)) != pmr::string::npos)
		{
			out.push_back(value.substr(0, pos));
			value.erase(0, pos + delim.length());
		}
		out.push_back(value);
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch
	> ML_NODISCARD static inline bool is_whitespace(Ch c) noexcept
	{
		return
			c == static_cast<Ch>(' ') ||
			c == static_cast<Ch>('\n') ||
			c == static_cast<Ch>('\t') ||
			c == static_cast<Ch>('\r');
	}

	ML_NODISCARD static inline pmr::string trim_front(pmr::string value) noexcept
	{
		while (!value.empty() && is_whitespace(value.front()))
		{
			value.erase(value.begin());
		}
		return value;
	}

	ML_NODISCARD static inline pmr::string trim_back(pmr::string value) noexcept
	{
		while (!value.empty() && is_whitespace(value.back()))
		{
			value.pop_back();
		}
		return value;
	}

	ML_NODISCARD static inline pmr::string trim(pmr::string value) noexcept
	{
		return trim_front(trim_back(value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline pmr::string replace_all(pmr::string value, pmr::string const & toFind, pmr::string const & replaceWith) noexcept
	{
		if (!value.empty() && !toFind.empty())
		{
			for (size_t i = 0; (i = value.find(toFind, i)) != pmr::string::npos;)
			{
				value.replace(i, toFind.size(), replaceWith);
				i += replaceWith.size();
			}
		}
		return value;
	}

	ML_NODISCARD static inline pmr::string remove_all(pmr::string value, pmr::string const & toRemove) noexcept
	{
		return replace_all(value, toRemove, {});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline bool is_alpha(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::isalpha(elem, loc))
				return false;
		return true;
	}

	ML_NODISCARD static inline bool is_alnum(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::isalnum(elem, loc))
				return false;
		return true;
	}

	ML_NODISCARD static inline bool is_graph(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::isgraph(elem, loc))
				return false;
		return true;
	}

	ML_NODISCARD static inline bool is_print(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::isprint(elem, loc))
				return false;
		return true;
	}

	ML_NODISCARD static inline bool is_lower(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::islower(elem, loc))
				return false;
		return true;
	}

	ML_NODISCARD static inline bool is_upper(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		for (auto const & elem : value)
			if (!std::islower(elem, loc))
				return false;
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline pmr::string to_lower(pmr::string value) noexcept
	{
		std::locale loc{};
		for (auto & elem : value)
			elem = std::tolower(elem, loc);
		return value;
	}

	ML_NODISCARD static inline pmr::string to_upper(pmr::string value) noexcept
	{
		std::locale loc{};
		for (auto & elem : value)
			elem = std::toupper(elem, loc);
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline bool is_bool(pmr::string const & value) noexcept
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

	ML_NODISCARD static inline bool is_integer(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		auto it{ std::begin(value) };
		if ((*it) == '-') it++; 
		while (it != std::end(value) && std::isdigit(*it, loc)) ++it;
		return (it == std::end(value));
	}

	ML_NODISCARD static inline bool is_decimal(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		char * endptr{ nullptr };
		auto temp { std::strtod(value.c_str(), &endptr) };
		return !(*endptr != '\0' || endptr == value);
	}

	ML_NODISCARD static inline bool is_name(pmr::string const & value) noexcept
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

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<bool> to_bool(Str && str) noexcept
	{
		switch (hash(to_lower(str)))
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

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<int8_t> to_i8(Str && str, int32_t base = 10) noexcept
	{
		return impl::string_convert<int8_t>(impl::c_string(ML_FWD(str)), &_CSTD strtol, base);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<int16_t> to_i16(Str && str, int32_t base = 10) noexcept
	{
		return impl::string_convert<int16_t>(impl::c_string(ML_FWD(str)), &_CSTD strtol, base);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<int32_t> to_i32(Str && str, int32_t base = 10) noexcept
	{
		return impl::string_convert<int32_t>(impl::c_string(ML_FWD(str)), &_CSTD strtol, base);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<int64_t> to_i64(Str && str, int32_t base = 10) noexcept
	{
		return impl::string_convert<int64_t>(impl::c_string(ML_FWD(str)), &_CSTD strtoll, base);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<uint8_t> to_u8(Str && str, int32_t base = 10) noexcept
	{
		return impl::string_convert<uint8_t>(impl::c_string(ML_FWD(str)), &_CSTD strtoul, base);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<uint16_t> to_u16(Str && str, int32_t base = 10) noexcept
	{
		return impl::string_convert<uint16_t>(impl::c_string(ML_FWD(str)), &_CSTD strtoul, base);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<uint32_t> to_u32(Str && str, int32_t base = 10) noexcept
	{
		return impl::string_convert<uint32_t>(impl::c_string(ML_FWD(str)), &_CSTD strtoul, base);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<uint64_t> to_u64(Str && str, int32_t base = 10) noexcept
	{
		return impl::string_convert<uint64_t>(impl::c_string(ML_FWD(str)), &_CSTD strtoull, base);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<float32_t> to_f32(Str && str) noexcept
	{
		return impl::string_convert<float32_t>(impl::c_string(ML_FWD(str)), &_CSTD strtod);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<float64_t> to_f64(Str && str) noexcept
	{
		return impl::string_convert<float64_t>(impl::c_string(ML_FWD(str)), &_CSTD strtod);
	}

	template <class Str = pmr::string
	> ML_NODISCARD static inline std::optional<float80_t> to_f80(Str && str) noexcept
	{
		return impl::string_convert<float80_t>(impl::c_string(ML_FWD(str)), &_CSTD strtold);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline pmr::string to_string(int8_t const value) noexcept
	{
		return impl::integral_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(int16_t const value) noexcept
	{
		return impl::integral_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(int32_t const value) noexcept
	{
		return impl::integral_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(int64_t const value) noexcept
	{
		return impl::integral_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(uint8_t const value) noexcept
	{
		return impl::integral_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(uint16_t const value) noexcept
	{
		return impl::integral_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(uint32_t const value) noexcept
	{
		return impl::integral_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(uint64_t const value) noexcept
	{
		return impl::integral_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(float64_t const value) noexcept
	{
		return impl::floating_point_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(float32_t const value) noexcept
	{
		return impl::floating_point_to_string<char>(value);
	}

	ML_NODISCARD static inline pmr::string to_string(float80_t const value) noexcept
	{
		return impl::floating_point_to_string<char>(value);
	}

	template <class T
	> ML_NODISCARD static inline pmr::string to_string(T const & value) noexcept
	{
		std::stringstream ss{};
		ss << value;
		return pmr::string{ ss.str() };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Arg0, class ... Args
	> ML_NODISCARD static inline pmr::string format(pmr::string const & fmt, Arg0 const & arg0, Args && ... args) noexcept
	{
		std::stringstream ss{ impl::sink_stream(ML_FWD(arg0), ML_FWD(args)...) };
		return format(fmt, ss);
	}

	ML_NODISCARD static inline pmr::string format(pmr::string fmt, std::stringstream & ss) noexcept
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

	template <class S
	> ML_NODISCARD static inline pmr::string format(pmr::string fmt, pmr::vector<S> const & args) noexcept
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

	template <class T
	> ML_NODISCARD static inline pmr::string to_hex(T const & value)
	{
		std::stringstream ss{};
		ss << std::hex << value;
		return ss.str();
	}

	template <class T
	> ML_NODISCARD static inline pmr::string to_oct(T const & value)
	{
		std::stringstream ss{};
		ss << std::oct << value;
		return ss.str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STRING_UTILITY_HPP_