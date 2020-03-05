#ifndef _ML_JSON_HPP_
#define _ML_JSON_HPP_

#include <libmeme/Common.hpp>
#include <nlohmann/json.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_ALIAS json = typename nlohmann::json;

	template <class ... T> struct json_utility;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct jstream final
	{
		json m_data{};

		template <class T, class K
		> inline jstream & write(K && k, T const & v) noexcept
		{
			m_data[ML_FWD(k)] = v;
			return (*this);
		}

		template <class T, class K
		> inline jstream const & read(K && k, T & v) const noexcept
		{
			v = m_data[ML_FWD(k)].get<T>();
			return (*this);
		}

		ML_NODISCARD inline operator json & () & noexcept { return m_data; }

		ML_NODISCARD inline operator json const & () const & noexcept { return m_data; }

		ML_NODISCARD inline operator json && () && noexcept { return std::move(m_data); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct json_utility<> final
	{
		using self_type = typename json_utility<>;

		template <class T
		> static inline jstream & serialize(jstream & out, T const & v)
		{
			return std::invoke(json_utility<T>{}, out, v);
		}

		template <class T
		> static inline jstream const & deserialize(jstream const & in, T & v)
		{
			return std::invoke(json_utility<T>{}, in, v);
		}

		template <class T
		> static inline json to_json(T const & v)
		{
			jstream temp{};
			self_type::serialize(temp, v);
			return temp;
		}

		template <class T
		> static inline T from_json(jstream const & in)
		{
			T temp{};
			self_type::deserialize(in, temp);
			return temp;
		}

		template <class T
		> static inline T from_json(json const & j)
		{
			return self_type::from_json<T>(jstream{ j });
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct json_utility<T> final
	{
		inline auto & operator()(jstream & out, T const & v) noexcept
		{
			return out;
		}

		inline auto const & operator()(jstream const & in, T & v) noexcept
		{
			return in;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_JSON_HPP_