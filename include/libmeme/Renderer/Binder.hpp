#ifndef _ML_BINDER_HPP_
#define _ML_BINDER_HPP_

#include <libmeme/Core/ScopeGuard.hpp>

#define ML_BIND_SCOPE(ref, ...)	\
	ref.bind(##__VA_ARGS__);	\
	ML_DEFER{ ref.unbind(); }

#endif // !_ML_BINDER_HPP_