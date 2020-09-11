#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

#include <libmeme/Core/BatchVector.hpp>
#include <libmeme/Engine/SharedLibrary.hpp>
#include <libmeme/Engine/Plugin.hpp>

namespace ml
{
	struct ML_ENGINE_API application final : system_object<application>, event_listener
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

		explicit application(system_context * sys);

		~application() noexcept override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void on_event(event const & value) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		int32_t run();

		int32_t operator()() noexcept { return run(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		plugin_id install_plugin(fs::path const & path, void * user = {});

		bool uninstall_plugin(plugin_id value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		ML_NODISCARD static auto get() noexcept -> application * { return g_app; }

		ML_NODISCARD auto get_plugins() const noexcept -> plugin_manager const & { return m_plugins; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static application *	g_app		; // singleton
		bool					m_running	; // running
		plugin_manager			m_plugins	; // plugins

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_