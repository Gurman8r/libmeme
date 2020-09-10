#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

#include <libmeme/Core/BatchVector.hpp>
#include <libmeme/Engine/SharedLibrary.hpp>
#include <libmeme/Engine/Plugin.hpp>
#include <libmeme/Graphics/RenderWindow.hpp>

namespace ml
{
	struct ML_ENGINE_API application final : system_object<application>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct ML_NODISCARD plugin_api final
		{
			plugin * (*attach)(application *, void *);

			void (*detach)(application *, plugin *);
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit application(system_context * sys) noexcept;

		~application() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_id install(fs::path const & path, void * user = nullptr);

		bool uninstall(plugin_id value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		ML_NODISCARD static application * get() noexcept { return g_app; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static application * g_app; // instance

		ds::batch_vector<
			plugin_id		,	// id
			fs::path		,	// path
			shared_library	,	// library
			manual<plugin>	,	// instance
			plugin_api			// interface
		> m_plugins;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_