#ifndef _ML_INPUT_HPP_
#define _ML_INPUT_HPP_

#include <libmeme/Core/Debug.hpp>

namespace ml::impl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct input;

	template <> struct input<> final { input() = delete; };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<pmr::string> final
	{
		ML_NODISCARD inline auto operator()(std::istream & in) const noexcept
		{
			pmr::string temp{};
			if (in.good()) { in >> temp; }
			return temp;
		}

		ML_NODISCARD inline auto operator()(std::istream & in, char delim) const noexcept
		{
			pmr::string temp{};
			std::getline(in, temp, delim);
			return temp;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<bool> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, bool dv = {}) const noexcept
		{
			if (auto const o{ util::to_bool(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, bool dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int8_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, int8_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_i8(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, int8_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int16_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, int16_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_i16(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, int16_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int32_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, int32_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_i32(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, int32_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<int64_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, int64_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_i64(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, int64_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint8_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, uint8_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_u8(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, uint8_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint16_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, uint16_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_u16(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, uint16_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint32_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, uint32_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_u32(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, uint32_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<uint64_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, uint64_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_u64(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, uint64_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float32_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, float32_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_f32(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, float32_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float64_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, float64_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_f64(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, float64_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct input<float80_t> final
	{
		template <class Str = pmr::string
		> ML_NODISCARD inline auto operator()(Str && str, float80_t dv = {}) const noexcept
		{
			if (auto const o{ util::to_f80(ML_FWD(str)) }) { return (*o); }
			else { return dv; }
		}

		ML_NODISCARD inline auto operator()(std::istream & in, float80_t dv = {}) const noexcept
		{
			return (*this)(input<pmr::string>{}(in), dv);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct input<T> final
	{
		ML_NODISCARD inline auto operator()(std::istream & in) const noexcept
		{
			T temp{};
			if (in.good()) { in >> temp; }
			return temp;
		}

		ML_NODISCARD inline auto operator()(cstring str) const noexcept
		{
			std::stringstream ss{ str };
			return (*this)(ss);
		}

		ML_NODISCARD inline auto operator()(pmr::string const & str) const noexcept
		{
			return (*this)(str.c_str());
		}

		ML_NODISCARD inline auto operator()() const noexcept
		{
			return (*this)(debug::in());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	template <class T, class ... Args
	> ML_NODISCARD static inline T input(Args && ... args)
	{
		return std::invoke(impl::input<T>{}, ML_FWD(args)...);
	}
}

#endif // !_ML_INPUT_HPP_