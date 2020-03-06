#ifndef _ML_STYLE_LOADER_HPP_
#define _ML_STYLE_LOADER_HPP_

#include <libmeme/Editor/Export.hpp>
#include <libmeme/Core/NonCopyable.hpp>
#include <libmeme/Common.hpp>

namespace ml
{
	struct ML_EDITOR_API style_loader final : non_copyable
	{
		style_loader() noexcept = default;

		bool operator()(filesystem::path const & path);
	};
}

#endif // !_ML_STYLE_LOADER_HPP_