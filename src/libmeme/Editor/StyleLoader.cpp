#include <libmeme/Editor/StyleLoader.hpp>
#include <libmeme/Editor/ImGui.hpp>
#include <libmeme/Core/FileSystem.hpp>
#include <libmeme/Core/FlatMap.hpp>

namespace ml
{
	style_loader::style_loader(fs::path const & path)
	{
	}
	
	bool style_loader::operator()(fs::path const & path)
	{
		return false;
	}
}