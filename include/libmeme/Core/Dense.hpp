#ifndef _ML_DENSE_HPP_
#define _ML_DENSE_HPP_

#include <libmeme/Core/Core.hpp>

#define _ML_DENSE _ML dense::

// Traits
namespace ml::dense
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Elem, class Alloc
	> struct storage_traits
	{
		using value_type				= typename Elem;
		using allocator_type			= typename Alloc;
		using storage_type				= typename std::vector<value_type, allocator_type>;
		using initializer_type			= typename std::initializer_list<value_type>;
		using pointer					= typename storage_type::pointer;
		using reference					= typename storage_type::reference;
		using const_pointer				= typename storage_type::const_pointer;
		using const_reference			= typename storage_type::const_reference;
		using difference_type			= typename storage_type::difference_type;
		using size_type					= typename storage_type::size_type;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DENSE_HPP_