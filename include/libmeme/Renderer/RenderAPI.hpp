#ifndef _ML_RENDER_API_HPP_
#define _ML_RENDER_API_HPP_

#include <libmeme/Renderer/Export.hpp>
#include <libmeme/Core/Singleton.hpp>
#include <libmeme/Core/TypeOf.hpp>

namespace ml
{
	template <class ... T> struct RenderAPI;

	template <> struct RenderAPI<> { RenderAPI() = delete; };

	template <class Impl> struct RenderAPI<Impl>
	{
		using impl_type = typename Impl;
	};
}

#endif // !_ML_RENDER_API_HPP_