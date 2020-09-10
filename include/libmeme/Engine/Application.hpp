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

		using plugin_manager = typename ds::batch_vector
		<
			plugin_id		,	// id
			fs::path		,	// path
			shared_library	,	// library
			manual<plugin>	,	// instance
			plugin_api			// interface
		>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		explicit application(system_context * sys) noexcept;

		~application() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_id install_plugin(fs::path const & path, void * user = {});

		bool uninstall_plugin(plugin_id value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		ML_NODISCARD static auto get() noexcept -> application * { return g_app; }

		ML_NODISCARD auto get_plugins() const noexcept -> plugin_manager const & { return m_plugins; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static application *	g_app		; // instance
		plugin_manager			m_plugins	; // plugins

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_