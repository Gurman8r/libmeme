#ifndef _ML_JSON_HPP_
#define _ML_JSON_HPP_

#include <libmeme/Config.hpp>
#include <nlohmann/json.hpp>

namespace ml { ML_ALIAS json = typename nlohmann::json; }

#endif // !_ML_JSON_HPP_