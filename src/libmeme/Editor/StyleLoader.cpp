#include <libmeme/Editor/StyleLoader.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	style_loader::style_loader()
	{
	}
	
	bool style_loader::operator()(fs::path const & path)
	{
		if (auto const data{ FS::get_file_contents(path) }; !data.empty())
		{
			std::stringstream ss{ data.c_str() };
			return true;
		}
		return false;
	}
}