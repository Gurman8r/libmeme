#ifndef _ML_NON_NEWABLE_HPP_
#define _ML_NON_NEWABLE_HPP_

namespace ml
{
	class NonNewable
	{
		inline void * operator new(size_t) { return nullptr; }

		inline void * operator new[](size_t) { return nullptr; }

		inline void operator delete(void *) { return; }

		inline void operator delete[](void *) { return; }
	};
}

#endif // !_ML_NON_NEWABLE_HPP_