#ifndef _ML_APPLICATION_HPP_
#define _ML_APPLICATION_HPP_

#include <libmeme/Core/BatchVector.hpp>
#include <libmeme/Core/SharedLibrary.hpp>
#include <libmeme/Client/Plugin.hpp>

namespace ml
{
	struct ML_CLIENT_API application final : system_object<application>
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
			shared_library	,	// library instance
			manual<plugin>	,	// plugin instance
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

		plugin_id install_plugin(fs::path const & path, void * user = nullptr);

		bool uninstall_plugin(plugin_id value);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		ML_NODISCARD static application * get() noexcept { return g_app; }

		ML_NODISCARD auto get_plugins() const noexcept -> plugin_manager const & { return m_mods; }

		ML_NODISCARD bool has_plugin(fs::path const & path) const noexcept
		{
			return m_mods.contains<plugin_id>
			(
				ML_handle(plugin_id, util::hash(shared_library::format_path(path).string()))
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		static application *	g_app		; // singleton
		bool					m_running	; // running
		plugin_manager			m_mods		; // plugins

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_APPLICATION_HPP_