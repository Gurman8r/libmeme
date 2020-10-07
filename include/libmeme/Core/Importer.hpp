#ifndef _ML_IMPORTER_HPP_
#define _ML_IMPORTER_HPP_

// WIP

#include <libmeme/Core/Memory.hpp>

namespace ml
{
	template <class ... Ts
	> struct importer;

	template <> struct importer<>
	{
	};

	template <class T
	> struct importer<T>
	{
	};
}

#endif // !_ML_IMPORTER_HPP_