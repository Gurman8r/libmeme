#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/StyleLoader.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Scripting/Embed.hpp>

namespace ml::embedded
{
	PYBIND11_EMBEDDED_MODULE(LIBMEME_EDITOR, inst)
	{
		inst.def("load_style", [](cstring s) { style_loader()(s); })
			;
	};
}