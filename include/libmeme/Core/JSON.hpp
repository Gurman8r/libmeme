#ifndef _ML_JSON_HPP_
#define _ML_JSON_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Common.hpp>
#include <nlohmann/json.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	ML_alias json = typename nlohmann::json;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace std::filesystem
{
	static void to_json(_ML json & j, path const & value)
	{
		j = value.native();
	}

	static void from_json(_ML json const & j, path & value)
	{
		if (j.is_string()) { value = j.get<pmr::string>(); }
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_JSON_HPP_