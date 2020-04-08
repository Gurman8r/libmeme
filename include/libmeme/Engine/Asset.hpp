#ifndef _ML_ASSET_HPP_
#define _ML_ASSET_HPP_

// WIP

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	template <class T
	> struct asset_data
	{
		pmr::string name;
		hash_t guid;
		std::optional<T> data;
	};
}

#endif // !_ML_ASSET_HPP_