#include <libmeme/Editor/Editor.hpp>
#include <libmeme/Editor/StyleLoader.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(LIBMEME_EDITOR, m)
	{
		m /* LIBMEME_EDITOR */

			// style
			.def("load_style", [](cstring s) { return style_loader::load_from_file(s); })
			
			;
	};
}