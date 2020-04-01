#ifndef _ML_BINDER_HPP_
#define _ML_BINDER_HPP_

#include <libmeme/Core/ScopeGuard.hpp>

#define ML_bind_scope(value, ...)	\
	(value).bind(##__VA_ARGS__);	\
	ML_defer{ (value).unbind(); }

#endif // !_ML_BINDER_HPP_