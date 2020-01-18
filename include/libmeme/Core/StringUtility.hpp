#ifndef _ML_STRING_UTILITY_HPP_
#define _ML_STRING_UTILITY_HPP_

#include <libmeme/Core/Utility.hpp>

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

		default: return false;
		}
	}

	ML_NODISCARD static inline bool is_integer(pmr::string const & value) noexcept
	{
		if (value.empty()) return false; 
		std::locale loc{};
		pmr::string::const_iterator it = value.cbegin();
		if ((*it) == '-')
		{ 
			it++; 
		}
		while (it != value.cend() && std::isdigit(*it, loc))
		{
			++it;
		}
		return (it == value.cend());
	}

	ML_NODISCARD static inline bool is_decimal(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		pmr::string::pointer endptr = nullptr;
		auto temp { std::strtod(value.c_str(), &endptr) };
		return !(*endptr != '\0' || endptr == value);
	}

	ML_NODISCARD static inline bool is_name(pmr::string const & value) noexcept
	{
		if (value.empty()) return false;
		std::locale loc{};
		if (std::isalpha(value.front(), loc) || (value.front() == '_'))
		{
			for (size_t i = 1; i < value.size(); ++i)
			{
				if (value[i] == '_')
				{
					continue;
				}
				else if (!std::isalnum(value[i], loc))
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline bool to_bool(pmr::string const & value, bool dv = 0) noexcept
	{
		switch (hash(to_lower(value)))
		{
		case hash("1"):
		case hash("true"):
		case hash("on"):
		case hash("yes"):
			return true;

		case hash("0"):
		case hash("false"):
		case hash("off"):
		case hash("no"):
			return false;

		default:
			return dv;
		}
	}

	ML_NODISCARD static inline int32_t to_i8(pmr::string const & value, int8_t dv = 0) noexcept
	{
		try { return static_cast<int8_t>(std::stoi(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline int32_t to_i16(pmr::string const & value, int16_t dv = 0) noexcept
	{
		try { return static_cast<int16_t>(std::stoi(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline int32_t to_i32(pmr::string const & value, int32_t dv = 0) noexcept
	{
		try { return static_cast<int32_t>(std::stoi(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline int64_t to_i64(pmr::string const & value, int64_t dv = 0) noexcept
	{
		try { return static_cast<int64_t>(std::stoll(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline uint8_t to_u8(pmr::string const & value, uint8_t dv = 0) noexcept
	{
		try { return static_cast<uint8_t>(std::stoul(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline uint16_t to_u16(pmr::string const & value, uint16_t dv = 0) noexcept
	{
		try { return static_cast<uint16_t>(std::stoul(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline uint32_t to_u32(pmr::string const & value, uint32_t dv = 0) noexcept
	{
		try { return static_cast<uint32_t>(std::stoul(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline uint64_t to_u64(pmr::string const & value, uint64_t dv = 0) noexcept
	{
		try { return static_cast<uint64_t>(std::stoull(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline float32_t to_f32(pmr::string const & value, float32_t dv = 0) noexcept
	{
		try { return static_cast<float32_t>(std::stof(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline float64_t to_f64(pmr::string const & value, float64_t dv = 0) noexcept
	{
		try { return static_cast<float64_t>(std::stod(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	ML_NODISCARD static inline float80_t to_f80(pmr::string const & value, float80_t dv = 0) noexcept
	{
		try { return static_cast<float80_t>(std::stold(value.c_str())); }
		catch (std::invalid_argument &) { return dv; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_NODISCARD static inline pmr::string to_string(int8_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(int16_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(int32_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(int64_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(uint8_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(uint16_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(uint32_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(uint64_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(float32_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(float64_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	ML_NODISCARD static inline pmr::string to_string(float80_t value) noexcept
	{
		return std::to_string(value).c_str();
	}

	template <class T
	> ML_NODISCARD static inline pmr::string to_string(T const & value) noexcept
	{
		std::stringstream ss{};
		ss << value;
		return ss.str().c_str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Arg0, class ... Args
	> inline std::stringstream sink(Arg0 const & arg0, Args && ... args) noexcept
	{
		std::stringstream ss{};
		ss << arg0 << '\n';
		int32_t i[] = { 0, ((void)(ss << args << '\n'), 0)... }; (void)i;
		return ss;
	}

	template <class Arg0, class ... Args
	> ML_NODISCARD static inline pmr::string format(pmr::string fmt, Arg0 const & arg0, Args && ... args) noexcept
	{
		std::stringstream ss{ sink(arg0, std::forward<Args>(args)...) };
		return format(fmt, ss);
	}

	ML_NODISCARD static inline pmr::string format(pmr::string fmt, std::stringstream & ss) noexcept
	{
		for (size_t i = 0; ss.good(); ++i)
		{
			pmr::string line;
			if (std::getline(ss, line))
			{
				fmt = replace_all(fmt, ("{" + to_string(i) + "}"), line);
			}
		}
		return fmt;
	}

	template <class S> ML_NODISCARD static inline pmr::string format(pmr::string fmt, pmr::vector<S> const & args) noexcept
	{
		for (size_t i = 0, imax = args.size(); i < imax; ++i)
		{
			fmt = replace_all(fmt, ("{" + to_string(i) + "}"), args[i]);
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

#endif // !_ML_STRING_UTILITY_HPP_