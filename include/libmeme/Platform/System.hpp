#ifndef _ML_SYSTEM_HPP_
#define _ML_SYSTEM_HPP_

// WIP

#include <libmeme/Platform/Export.hpp>
#include <libmeme/Core/Singleton.hpp>

namespace ml
{
	struct ML_PLATFORM_API system final : singleton<system>
	{

	private:
		friend singleton<system>;

		~system() noexcept;
	};
}

#endif // !_ML_SYSTEM_HPP_