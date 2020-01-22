#ifndef _ML_BINDER_HPP_
#define _ML_BINDER_HPP_

#include <libmeme/Core/ScopeGuard.hpp>

// Bind Scope Member
#define ML_BIND_SCOPE(ref, ...) \
	ref.bind(##__VA_ARGS__); ML_DEFER{ ref.unbind(); }

// Bind Scope Static
#define ML_BIND_SCOPE_S(T, ptr, ...) \
	T::bind(ptr, ##__VA_ARGS__); ML_DEFER{ T::bind(nullptr); }

#endif // !_ML_BINDER_HPP_