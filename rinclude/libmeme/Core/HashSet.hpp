#ifndef _ML_HASH_SET_HPP_
#define _ML_HASH_SET_HPP_

// WIP

#include <libmeme/Core/FlatMap.hpp>

namespace ml::ds
{
	template <class _Kt
	> struct hash_set final
	{
		using value_type	= typename _Kt;
		using self_type		= typename hash_set<value_type>;
		using storage_type	= typename map<hash_t, value_type>;

		storage_type m_data;
	};
}

#endif // !_ML_HASH_SET_HPP_