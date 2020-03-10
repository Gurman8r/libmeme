#include <libmeme/Engine/Engine.hpp>

#define ML_EMBED_PYTHON
#include <libmeme/Engine/Embed.hpp>

namespace ml::embed
{
	PYBIND11_EMBEDDED_MODULE(LIBMEME_ENGINE, m)
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		m /* FUNCTIONS */
			;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}