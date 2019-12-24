#ifndef _ML_RING_BUFFER_HPP_
#define _ML_RING_BUFFER_HPP_

#include <libmeme/Core/Dense.hpp>

namespace ml
{
	template <class T> struct RingBuffer
	{
		using value_type = typename T;
	};
}

#endif // !_ML_RING_BUFFER_HPP_