#ifndef _ML_EMBED_EXPORT_HPP_
#define _ML_EMBED_EXPORT_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <libmeme/Config.hpp>

#if defined(ML_API)
#	define ML_EMBED_API ML_API
#elif defined(ML_EMBED_EXPORTS)
#	define ML_EMBED_API ML_API_EXPORT
#else
#	define ML_EMBED_API ML_API_IMPORT
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_EMBED_EXPORT_HPP_