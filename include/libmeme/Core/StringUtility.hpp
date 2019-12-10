#ifndef _ML_STRING_UTILITY_HPP_
#define _ML_STRING_UTILITY_HPP_

#include <libmeme/Core/TypeOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace util
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline std::vector<std::string> tokenize(std::string value, std::string const & delim)
		{
			std::vector<std::string> out;
			size_t pos = 0;
			while ((pos = value.find(delim)) != std::string::npos)
			{
				out.push_back(value.substr(0, pos));
				value.erase(0, pos + delim.length());
			}
			out.push_back(value);
			return out;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class To, class From
		> static inline std::basic_string<To> convert(const std::basic_string<From> & value)
		{
			std::basic_string<To> temp {};
			temp.reserve(value.size());
			for (From const & c : value)
				temp.push_back(static_cast<To>(c));
			return temp;
		}

		static inline std::string narrow(std::wstring const & value)
		{ 
			return convert<char>(value);
		}

		static inline std::wstring widen(std::string const & value)
		{ 
			return convert<wchar_t>(value); 
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class Ch>
		static inline bool is_whitespace(Ch c)
		{
			return
				c == static_cast<Ch>(' ') ||
				c == static_cast<Ch>('\n') ||
				c == static_cast<Ch>('\t') ||
				c == static_cast<Ch>('\r');
		}

		static inline std::string trim_front(std::string value)
		{
			while (!value.empty() && is_whitespace(value.front()))
			{
				value.erase(value.begin());
			}
			return value;
		}

		static inline std::string trim_back(std::string value)
		{
			while (!value.empty() && is_whitespace(value.back()))
			{
				value.pop_back();
			}
			return value;
		}

		static inline std::string trim(std::string value)
		{
			return trim_front(trim_back(value));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline std::string replace_all(std::string value, std::string const & toFind, std::string const & replaceWith)
		{
			if (!value.empty() && !toFind.empty())
			{
				for (size_t i = 0; (i = value.find(toFind, i)) != std::string::npos;)
				{
					value.replace(i, toFind.size(), replaceWith);
					i += replaceWith.size();
				}
			}
			return value;
		}

		static inline std::string remove_all(std::string value, std::string const & toRemove)
		{
			return replace_all(value, toRemove, {});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool is_alpha(std::string const & value)
		{
			if (value.empty()) return false;
			std::locale loc {};
			for (auto const & elem : value)
				if (!std::isalpha(elem, loc))
					return false;
			return true;
		}

		static inline bool is_alnum(std::string const & value)
		{
			if (value.empty()) return false;
			std::locale loc {};
			for (auto const & elem : value)
				if (!std::isalnum(elem, loc))
					return false;
			return true;
		}

		static inline bool is_graph(std::string const & value)
		{
			if (value.empty()) return false;
			std::locale loc {};
			for (auto const & elem : value)
				if (!std::isgraph(elem, loc))
					return false;
			return true;
		}

		static inline bool is_print(std::string const & value)
		{
			for (auto const & elem : value)
				if (!std::isprint(elem))
					return false;
			return true;
		}

		static inline bool is_lower(std::string const & value)
		{
			for (auto const & elem : value)
				if (!std::islower(elem))
					return false;
			return true;
		}

		static inline bool is_upper(std::string const & value)
		{
			for (auto const & elem : value)
				if (!std::islower(elem))
					return false;
			return true;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline std::string to_lower(std::string value)
		{
			std::locale loc {};
			for (auto & elem : value)
				elem = std::tolower(elem, loc);
			return value;
		}

		static inline std::string to_upper(std::string value)
		{
			std::locale loc {};
			for (auto & elem : value)
				elem = std::toupper(elem, loc);
			return value;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool is_bool(std::string const & value)
		{
			
			switch (Hash(to_lower(value)))
			{
			case Hash("1"):
			case Hash("true"):
			case Hash("on"):
			case Hash("yes"):
			case Hash("0"):
			case Hash("false"):
			case Hash("off"):
			case Hash("no"):
				return true;

			default: return false;
			}
		}

		static inline bool is_integer(std::string const & value)
		{
			if (value.empty()) return false; 
			std::string::const_iterator it = value.cbegin();
			if ((*it) == '-')
			{ 
				it++; 
			}
			while (it != value.cend() && std::isdigit(*it))
			{
				++it;
			}
			return (it == value.cend());
		}

		static inline bool is_decimal(std::string const & value)
		{
			if (value.empty()) { return false; }
			std::string::pointer endptr = nullptr;
			auto temp { std::strtod(value.c_str(), &endptr) };
			return !(*endptr != '\0' || endptr == value);
		}

		static inline bool is_name(std::string const & value)
		{
			if (!value.empty() && (std::isalpha(value.front()) || (value.front() == '_')))
			{
				for (size_t i = 1; i < value.size(); i++)
				{
					if (value[i] == '_')
					{
						continue;
					}
					else if (!std::isalnum(value[i]))
					{
						return false;
					}
				}
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline bool to_bool(std::string const & value, bool dv = 0)
		{
			switch (Hash(to_lower(value)))
			{
			case Hash("1"):
			case Hash("true"):
			case Hash("on"):
			case Hash("yes"):
				return true;

			case Hash("0"):
			case Hash("false"):
			case Hash("off"):
			case Hash("no"):
				return false;
			
			default: return dv;
			}
		}

		static inline int32_t to_i8(std::string const & value, int8_t dv = 0)
		{
			try { return static_cast<int8_t>(std::stoi(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline int32_t to_i16(std::string const & value, int16_t dv = 0)
		{
			try { return static_cast<int16_t>(std::stoi(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline int32_t to_i32(std::string const & value, int32_t dv = 0)
		{
			try { return static_cast<int32_t>(std::stoi(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline int64_t to_i64(std::string const & value, int64_t dv = 0)
		{
			try { return static_cast<int64_t>(std::stoll(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline uint8_t to_u8(std::string const & value, uint8_t dv = 0)
		{
			try { return static_cast<uint8_t>(std::stoul(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline uint16_t to_u16(std::string const & value, uint16_t dv = 0)
		{
			try { return static_cast<uint16_t>(std::stoul(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline uint32_t to_u32(std::string const & value, uint32_t dv = 0)
		{
			try { return static_cast<uint32_t>(std::stoul(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline uint64_t to_u64(std::string const & value, uint64_t dv = 0)
		{
			try { return static_cast<uint64_t>(std::stoull(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline float32_t to_f32(std::string const & value, float32_t dv = 0)
		{
			try { return static_cast<float32_t>(std::stof(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline float64_t to_f64(std::string const & value, float64_t dv = 0)
		{
			try { return static_cast<float64_t>(std::stod(value)); }
			catch (std::exception &) { return dv; }
		}

		static inline float80_t to_f80(std::string const & value, float80_t dv = 0)
		{
			try { return static_cast<float80_t>(std::stold(value)); }
			catch (std::exception &) { return dv; }
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static inline std::string to_string(std::string const & value) { return value; }

		static inline std::string to_string(int8_t value) { return std::to_string(value); }

		static inline std::string to_string(int16_t value) { return std::to_string(value); }

		static inline std::string to_string(int32_t value) { return std::to_string(value); }

		static inline std::string to_string(int64_t value) { return std::to_string(value); }

		static inline std::string to_string(uint8_t value) { return std::to_string(value); }

		static inline std::string to_string(uint16_t value) { return std::to_string(value); }

		static inline std::string to_string(uint32_t value) { return std::to_string(value); }

		static inline std::string to_string(uint64_t value) { return std::to_string(value); }

		static inline std::string to_string(float32_t value) { return std::to_string(value); }

		static inline std::string to_string(float64_t value) { return std::to_string(value); }

		static inline std::string to_string(float80_t value) { return std::to_string(value); }

		template <class T> static inline std::string to_string(T const & value)
		{
			std::stringstream ss {}; 
			ss << value;
			return ss.str();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Arg0, class ... Args
		> inline std::stringstream sink(Arg0 const & arg0, Args && ... args)
		{
			std::stringstream ss{}; ss << arg0 << '\n';
			int32_t i[] = { 0, ((void)(ss << args << '\n'), 0)... }; (void)i;
			return ss;
		}

		static inline std::string format(std::string fmt, std::stringstream & ss)
		{
			for (size_t i = 0; ss.good(); i++)
			{
				std::string line;
				if (std::getline(ss, line))
				{
					fmt = replace_all(fmt, ("{" + to_string(i) + "}"), line);
				}
			}
			return fmt;
		}

		template <
			class Arg0, class ... Args
		> static inline std::string format(std::string fmt, Arg0 const & arg0, Args && ... args)
		{
			std::stringstream ss{ sink(arg0, std::forward<Args>(args)...) };
			
			return format(fmt, ss);
		}

		template <class S> static inline std::string format(std::string fmt, std::vector<S> const & args)
		{
			for (size_t i = 0, imax = args.size(); i < imax; i++)
			{
				fmt = replace_all(fmt, ("{" + to_string(i) + "}"), args[i]);
			}
			return fmt;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static inline std::string to_hex(T const & value)
		{
			std::stringstream ss {};
			ss << std::hex << value;
			return ss.str();
		}

		template <class T> static inline std::string to_oct(T const & value)
		{
			std::stringstream ss {};
			ss << std::oct << value;
			return ss.str();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STRING_UTILITY_HPP_