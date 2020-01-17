#ifndef _ML_INPUT_HPP_
#define _ML_INPUT_HPP_

#include <libmeme/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct input;

	template <> struct input<> final { input() = delete; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct input<T> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::istream & in) const
		{
			T temp{};
			if (in.good()) { in >> temp; }
			return temp;
		}

		ML_NODISCARD inline decltype(auto) operator()(std::string const & str) const
		{
			std::stringstream ss{ str };
			return (*this)(ss);
		}

		ML_NODISCARD inline decltype(auto) operator()() const
		{
			return (*this)(std::cin);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class To, class From> struct input<To, From> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, To dv = To { 0 }) const
		{
			return static_cast<To>(input<From>{}(str, static_cast<From>(dv)));
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, To dv = To { 0 }) const
		{
			return static_cast<To>(input<From>{}(in, static_cast<From>(dv)));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<std::string> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::istream & in) const
		{
			std::string temp{};
			if (in.good()) { in >> temp; }
			return temp;
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, char delim)
		{
			std::string temp{};
			std::getline(in, temp, delim);
			return temp;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<bool> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str) const
		{
			return util::to_bool(str);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in) const
		{
			return (*this)(input<std::string>{}(in));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int8_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, int8_t dv = 0) const
		{
			return util::to_i8(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, int8_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int16_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, int16_t dv = 0) const
		{
			return util::to_i16(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, int16_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int32_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, int32_t dv = 0) const
		{
			return util::to_i32(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, int32_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int64_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, int64_t dv = 0) const
		{
			return util::to_i64(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, int64_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint8_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, uint8_t dv = 0) const
		{
			return util::to_u8(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, uint8_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint16_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, uint16_t dv = 0) const
		{
			return util::to_u16(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, uint16_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint32_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, uint32_t dv = 0) const
		{
			return util::to_u32(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, uint32_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint64_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, uint64_t dv = 0) const
		{
			return util::to_u64(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, uint64_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float32_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, float32_t dv = 0) const
		{
			return util::to_f32(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, float32_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float64_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, float64_t dv = 0) const
		{
			return util::to_f64(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, float64_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float80_t> final
	{
		ML_NODISCARD inline decltype(auto) operator()(std::string const & str, float80_t dv = 0) const
		{
			return util::to_f80(str, dv);
		}

		ML_NODISCARD inline decltype(auto) operator()(std::istream & in, float80_t dv = 0) const
		{
			return (*this)(input<std::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_INPUT_HPP_