#ifndef _ML_SCRIPT_HPP_
#define _ML_SCRIPT_HPP_

#include <libmeme/Engine/Export.hpp>
#include <libmeme/Core/Memory.hpp>

namespace ml
{
	struct ML_ENGINE_API script : trackable, non_copyable
	{
		virtual ~script() noexcept override = default;
	};

	struct ML_ENGINE_API native_script : script
	{
		native_script() : script{}
		{
		}
	};
}

#endif // !_ML_SCRIPT_HPP_