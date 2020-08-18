#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <libmeme/System/Memory.hpp>

#define ML_register_type_ex(type, fn)		\
	static type * fn();						\
	bool _ML registrar<type>::s_registered	\
	{										\
		_ML registry::registrate<type>(&fn)	\
	};										\
	type * fn()

#define ML_register_type(type) \
	ML_register_type_ex(type, ML_cat(factory_, type))

namespace ml
{
	template <class T> class registrar final
	{
	protected: static bool s_registered;
	};

	// registry singleton
	class registry final : public singleton<registry>
	{
	public:
		template <class T, class Fn
		> static bool registrate(Fn && fn)
		{
			return true;
		}

	private:
		friend singleton;

		~registry() noexcept;
	};
}

#endif // !_ML_REGISTRY_HPP_